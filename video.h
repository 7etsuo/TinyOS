#ifndef VIDEO_H
#define VIDEO_H

#include "types.h"

#define MONO 0x02
#define get_video_base() ((UINT8 *)VIDEO_BASE)

extern IO_PORT8 VIDEO_BASE_HI;
extern IO_PORT8 VIDEO_BASE_MID;
extern IO_PORT8 VIDEO_BASE_REG;
extern IO_PORT8 VIDEO_SYNC_REG;
extern IO_PORT8 VIDEO_PAL0_REG;
extern IO_PORT8 VIDEO_REZ_REG;

/* UINT8 *get_video_base(); */
extern void clear_screen(UINT8 *base);
extern void scroll();
void plot_glyph(UINT8 ch);
void invert_cursor();
void reset_cursor();
void clear_cursor();
void init_video();

#endif /* VIDEO_H */

