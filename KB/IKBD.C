#include "CONIO.H"
#include "../HAL/ACIA.H"
#include "../KB/IKBD.H"
#include "../SCHD/PROC.H"
#include "../HAL/MEM_MAP.H"

IO_PORT8    IKBD_ACIA_CR		= (IO_PORT8) IKBD_ACIA_CR_ADDR;
IO_PORT8_RO IKBD_ACIA_SR		= (IO_PORT8_RO) IKBD_ACIA_SR_ADDR ;
IO_PORT8_RO IKBD_ACIA_RDR		= (IO_PORT8_RO) IKBD_ACIA_RDR_ADDR;

UINT16 * const kybd_buff_head		= (UINT16 * const) KYBD_BUFF_HEAD_ADDR;
UINT16 * const kybd_buff_tail		= (UINT16 * const) KYBD_BUFF_TAIL_ADDR;
UINT16 * const kybd_buff_fill		= (UINT16 * const) KYBD_BUFF_FILL_ADDR;
UINT16 * const kybd_num_lines      	= (UINT16 * const) KYBD_NUM_LINES_ADDR;
UINT16 * const kybd_len_line       	= (UINT16 * const) KYBD_LEN_LINE_ADDR; 
UINT16 * const kybd_shifted        	= (UINT16 * const) KYBD_SHIFTED_ADDR;
UINT8  * const kybd_auto_ch        	= (UINT8 * const)  KYBD_AUTO_CH_ADDR;
UINT16 * const kybd_auto_count     	= (UINT16 * const) KYBD_AUTO_COUNT_ADDR;
UINT16 * const kybd_blocked_proc   	= (UINT16 * const) KYBD_BLOCKED_PROC_ADDR;
UINT16 * const kybd_fg_proc        	= (UINT16 * const) KYBD_FG_PROC_ADDR;
UINT8  * const kybd_buff           	= (UINT8 * const)  KYBD_BUFF_ADDR;

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

