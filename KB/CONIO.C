#include "IKBD.H"
#include "CONIO.H"
#include "../TYPES.H"
#include "../SCHD/PROC.H"
#include "../SCHD/SCHED.H"
#include "../INT/ISR.H"
#include "../SCHD/CPU.H"
#include "../RASTER/FONT.H"
#include "../VID/VIDEO.H"
#include "../HAL/MEM_MAP.H"

UINT8 * const console_x_p		= (UINT8 * const) CONSOLE_X_P_ADDR; 
UINT8 * const console_y_p		= (UINT8 * const) CONSOLE_Y_P_ADDR;
UINT16 * const cursor_visible		= (UINT16 * const) CURSOR_VISIBLE_ADDR;

void print_char_safe(char ch)
{
	UINT16 orig_ipl = set_ipl(7);     /* [TO DO] mask less aggressively? */
	print_char(ch);
	(void) set_ipl(orig_ipl);
}

void print_char(char ch)
{
	/* [TO DO] mask VBL for cursor blink?  Already handled elsewhere? */

	/* [TO DO] not currently handled:
	   BEL(7),
	   TAB(9),
	   VT(11),
	   DEL(127)
	   */

	if (IS_PRINTABLE(ch))
	{
		plot_glyph(ch);
		(*console_x_p)++;

		if (*console_x_p == 80)
		{
			*console_x_p = 0;
			(*console_y_p)++;
		}

		*cursor_visible = 0;
	}
	else if (ch == '\b')
	{
		clear_cursor();

		if (*console_x_p > 0)
			(*console_x_p)--;
		else
		{
			(*console_x_p) = 79;
			(*console_y_p)--;
		}

		plot_glyph(' ');
	}
	else if (ch == CHAR_LF)					     /* LINE FEED (LF) */
	{
		clear_cursor();
		(*console_y_p)++;
	}
	else if (ch == CHAR_CR)					     /* CR */
	{
		clear_cursor();
		(*console_x_p) = 0;
	}
	else if (ch == CHAR_FF)					     /* FORM FEED (FF) */
	{
		clear_screen(get_video_base());
		*console_x_p = 0;
		*console_y_p = 0;
		*cursor_visible = 0;
	}

	if (*console_y_p == 50)
	{
		/* clear_cursor(); <-- [TO DO] needed? */
		(*console_y_p)--;
		scroll();
	}

	reset_cursor();
}

void print_str_safe(char *str)
{
	UINT16 orig_ipl = set_ipl(7);     /* [TO DO] mask less aggressively? */
	print_str(str);
	set_ipl(orig_ipl);
}

void print_str(char *str)
{
	register char ch;

	while ((ch = *(str++)))
		print_char(ch);
}

void init_console()
{
	print_char(CHAR_FF);                                   /* form feed inits console output driver */
	print_str("Welcome to Tiny OS (kernel v0.2).\r\n");

	*kybd_isr_state = 0;
	*kybd_buff_head = 1;
	*kybd_buff_tail = 0;
	*kybd_buff_fill = 0;
	*kybd_num_lines = 0;
	*kybd_len_line = 0;
	*kybd_shifted = 0;
	*kybd_auto_ch = 0;
	*kybd_auto_count = 0;
	*kybd_blocked_proc = -1;
	*kybd_fg_proc = 0; /* sets the kybd_fg_proc to the 0th program */
}

void do_write(const char *buf, unsigned int len)
{
	/* [TO DO] validate buf */

	for (; len > 0; len--)
		print_char_safe(*(buf++));
}

int do_read(char *buf, unsigned int len)
{
	int num_read = 0, cr_hit = 0;
	UINT16 orig_ipl = set_ipl(6);   /* need to mask both MFP IRQ and VBL IRQ */
									/* [TO DO] mask above less aggressively */

	/* [TO DO] validate buf */

	if (*curr_proc == *kybd_fg_proc)
		if (*kybd_num_lines)
			while (len > 0 && !cr_hit)
			{
				if ((buf[num_read++] = kybd_buff[*kybd_buff_head]) == '\r')
				{
					(*kybd_num_lines)--;
					cr_hit = 1;
				}

				*kybd_buff_head = (*kybd_buff_head + 1) & 127;
				(*kybd_buff_fill)--;
				len--;
			}
		else
		{
			*kybd_blocked_proc = *curr_proc;
			CURR_PROC->state = PROC_BLOCKED;
			*resched_needed = YES_BLOCK;		/* signals the trap will need to be restarted */
												/* [TO DO] examine completing system call in bottom half, instead */	
		}
	else
	{
		print_str_safe("read attempted by background process ");
		print_char_safe((char)(CURR_PROC->pid + '0'));
		print_str_safe("\r\n");

		terminate();
	}

	set_ipl(orig_ipl);

	return num_read;
}

