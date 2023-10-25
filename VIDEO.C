#include "video.h"
#include "conio.h"
#include "font.h"
#include "irq.h"
#include "types.h"
#include "mem_map.h"

#define TEXT_OFFSET (get_video_base() + (UINT16)*console_y_p*640 + *console_x_p)

IO_PORT8 VIDEO_BASE_HI  = (IO_PORT8) VIDEO_BASE_HI_ADDR;
IO_PORT8 VIDEO_BASE_MID = (IO_PORT8) VIDEO_BASE_MID_ADDR;
IO_PORT8 VIDEO_BASE_REG = (IO_PORT8) VIDEO_BASE_REG_ADDR;
IO_PORT8 VIDEO_SYNC_REG = (IO_PORT8) VIDEO_SYNC_REG_ADDR;
IO_PORT8 VIDEO_PAL0_REG = (IO_PORT8) VIDEO_PAL0_REG_ADDR;
IO_PORT8 VIDEO_REZ_REG  = (IO_PORT8) VIDEO_REZ_REG_ADDR;

/*
   UINT8 *get_video_base()
   {
   return (UINT8 *)(
   ((UINT32)*VIDEO_BASE_HI << 16) +
   ((UINT32)*VIDEO_BASE_MID << 8)
   );
   }
   */

void plot_glyph(UINT8 ch)
{
	UINT8 *dst = TEXT_OFFSET;
	const UINT8 *src = GLYPH_START(ch);
	int i;

	for (i = 0; i < 8; i++)
	{
		*dst = *(src++);
		dst += 80;
	}
}

void invert_cursor()
{
	UINT8 *base = TEXT_OFFSET;
	int i;

	for (i = 0; i < 8; i++)
	{
		*base ^= 0xFF;
		base += 80;
	}

	*cursor_visible = !(*cursor_visible);
}

void reset_cursor()
{
	if (!(*cursor_visible))
		invert_cursor();

	*vbl_counter = 0;
}

void clear_cursor()
{
	if (*cursor_visible)
		invert_cursor();
}

/* [TODO] add argument to video_base for double buffering */
void init_video()
{	
	/* move.l #VIDEO_BASE,d0 */ 
	/* lsr.l #8,d0 */ 
	UINT32 video_base_shifted = VIDEO_BASE >> 8;

	/* movea.l #VIDEO_BASE_REG,a0 */
	/* movep.w d0,0(a0) */
	*VIDEO_BASE_HI = (video_base_shifted >> 8) & 0xFF; /* High byte */
	*VIDEO_BASE_MID = video_base_shifted & 0xFF;       /* Medium byte */

	/* clr.b VIDEO_SYNC_REG */
	*VIDEO_SYNC_REG = 0; /*  initialize the sync register */

	/* move.w #1,VIDEO_PAL0_REG */
	*VIDEO_PAL0_REG = 1; /* init pallette to color 1 */

	/* move.b #MONO,VIDEO_REZ_REG */
	*VIDEO_REZ_REG = MONO; /* set video resolution to monochrome */
}

