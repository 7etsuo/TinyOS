#include "ikbd.h"
#include "mfp.h"
#include "acia.h"
#include "conio.h"
#include "proc.h"

IO_PORT8    IKBD_ACIA_CR			= (IO_PORT8) IKBD_ACIA_CR_ADDR;
IO_PORT8_RO IKBD_ACIA_SR  			= (IO_PORT8_RO) IKBD_ACIA_SR_ADDR ;
IO_PORT8_RO IKBD_ACIA_RDR 			= (IO_PORT8_RO) IKBD_ACIA_RDR_ADDR;

UINT16 * const kybd_isr_state       = (UINT16 * const) KYBD_ISR_STATE_ADDR; /* 0=not in mouse packet, 1=expecting delta x, 2=expecting delta y */
UINT16 * const kybd_buff_head       = (UINT16 * const) KYBD_BUFF_HEAD_ADDR;
UINT16 * const kybd_buff_tail       = (UINT16 * const) KYBD_BUFF_TAIL_ADDR;
UINT16 * const kybd_buff_fill       = (UINT16 * const) KYBD_BUFF_FILL_ADDR;
UINT16 * const kybd_num_lines       = (UINT16 * const) KYBD_NUM_LINES_ADDR;
UINT16 * const kybd_len_line        = (UINT16 * const) KYBD_LEN_LINE_ADDR; /* number of characters in buffer for current line */
UINT16 * const kybd_shifted         = (UINT16 * const) KYBD_SHIFTED_ADDR;
UINT8  * const kybd_auto_ch         = (UINT8 * const)  KYBD_AUTO_CH_ADDR;
UINT16 * const kybd_auto_count      = (UINT16 * const) KYBD_AUTO_COUNT_ADDR;
UINT16 * const kybd_blocked_proc    = (UINT16 * const) KYBD_BLOCKED_PROC_ADDR;
UINT16 * const kybd_fg_proc         = (UINT16 * const) KYBD_FG_PROC_ADDR;
UINT8  * const kybd_buff            = (UINT8 * const)  KYBD_BUFF_ADDR;  /* 128 byte circular queue - must be a power of 2 */

const UINT8 scan2ascii[2][128] =
{
	{	/* unshifted */
		0,    0,  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',     /* [TO DO] handle control characters? */
		'9',  '0',  '-',  '=', '\b', '\t',  'q',  'w',  'e',  'r',
		't',  'y',  'u',  'i',  'o',  'p',  '[',  ']', '\r',    0,
		'a',  's',  'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';',
		'\'',  '`',    0, '\\',  'z',  'x',  'c',  'v',  'b',  'n',
		'm',  ',',  '.',  '/',    0,    0,    0,  ' ',    0,    0,
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
		0,    0,    0,    0,    0,    0,    0,    0
	},
	{	/* shifted */
		0,    0,  '!',  '@',  '#',  '$',  '%',  '^',  '&',  '*',
		'(',  ')',  '_',  '+', '\b', '\t',  'Q',  'W',  'E',  'R',
		'T',  'Y',  'U',  'I',  'O',  'P',  '{',  '}', '\r',    0,
		'A',  'S',  'D',  'F',  'G',  'H',  'J',  'K',  'L',  ':',
		'\"', '~',    0,  '|',  'Z',  'X',  'C',  'V',  'B',  'N',
		'M',  '<',  '>',  '?',    0,    0,    0,  ' ',    0,    0,
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
		0,    0,    0,    0,    0,    0,    0,    0
	}
};


void do_ikbd_isr()
{
	/* [TO DO] allow unbuffered (raw) mode? */
	/* [TO DO] implement caps lock */
	/* [TO DO] beep if buffer full? */
	/* [TO DO] tweak to minimize chance of overrruns (algorithm, assembly, IRQ prioritization) */

	if (*IKBD_ACIA_SR & ACIA_IRQ)
	{
		UINT8 data = *IKBD_ACIA_RDR;

		switch (*kybd_isr_state)
		{
			case 0:
				if ((data & 0xFC) == 0xF8)
					*kybd_isr_state = 1;
				else if (data == 0x2A || data == 0x36)     /* [TO DO] incorporate shifting into autorepeat logic */
					(*kybd_shifted)++;
				else if (data == 0xAA || data == 0xB6)
					(*kybd_shifted)--;
				else if (!(data & 0x80))
				{
					*kybd_auto_ch = scan2ascii[*kybd_shifted ? 1 : 0][data];

					if (*kybd_auto_ch)
					{
						*kybd_auto_count = 0;
						input_enqueue(*kybd_auto_ch);
					}
				}
				else
					*kybd_auto_ch = 0;

				break;

			case 1:
				*kybd_isr_state = 2;
				break;

			case 2:
				*kybd_isr_state = 0;
				break;
		}

		/* [TO DO] write a "reset IKBD ACIA" function */

		if (*IKBD_ACIA_SR & ACIA_OVRN)     /* overrun during this slow ISR?  IRQ will still be asserted! */
		{
			*IKBD_ACIA_CR = 0x17;     /* reset the ACIA to clear IRQ, or the MFP won't re-ack the IRQ ... */
			*IKBD_ACIA_CR = 0x96;     /* ... and the IKBD will hang!                                      */
		}
	}

	*MFP_ISRB &= ~MFP_GPIP4;
}

void input_enqueue(char ch)
{
	if (ch == '\b')
	{
		if (*kybd_len_line > 0)
		{
			(*kybd_len_line)--;
			*kybd_buff_tail = (*kybd_buff_tail + 127) & 127;
			(*kybd_buff_fill)--;
			print_char(ch);
		}
	}
	else if (*kybd_buff_fill < 128)
	{
		if (ch == '\r')     /* [TO DO] if buffer almost full, a final '\r' should fit */
		{
			(*kybd_num_lines)++;
			*kybd_len_line = 0;

			if (*kybd_blocked_proc != -1)
			{
				proc[*kybd_blocked_proc].state = PROC_READY;
				*kybd_blocked_proc = -1;
			}
		}
		else
			(*kybd_len_line)++;

		*kybd_buff_tail = (*kybd_buff_tail + 1) & 127;
		kybd_buff[*kybd_buff_tail] = ch;
		(*kybd_buff_fill)++;	
		print_char(ch);
	}
}

