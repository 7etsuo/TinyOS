#ifndef VIDEO_H
#define VIDEO_H

#include "../TYPES.H"
#include "../HAL/MEM_MAP.H"

#define get_video_base() ((UINT8 *)VIDEO_BASE)
#define MONO 0x02

extern IO_PORT8 VIDEO_BASE_HI;
extern IO_PORT8 VIDEO_BASE_MID;
extern IO_PORT8 VIDEO_BASE_REG;
extern IO_PORT8 VIDEO_SYNC_REG;
extern IO_PORT8 VIDEO_PAL0_REG;
extern IO_PORT8 VIDEO_REZ_REG;

/* UINT8 *get_video_base(); */
extern void clear_screen(UINT8 *base);
extern void scroll(void);

void plot_glyph(UINT8 ch);
void invert_cursor(void);
void reset_cursor(void);
void clear_cursor(void);
void init_video(void);

#endif /* VIDEO_H */

