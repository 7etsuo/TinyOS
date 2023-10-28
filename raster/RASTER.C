#include "../types.h"
#include "font.h"
#include "raster.h"

static inline int CalcRowOffset(int y, int col_width) {
	return y * col_width;
}

static inline int CalcOffset(int row, int col, int col_width)
{
	return CalcRowOffset(row, col_width) + col;
}

static inline void CommonRasterOperation(const UINT8 *frame_buffer, Sprite *sprite, UINT8 (*operation)(UINT8, UINT8))
{
	int i;
	int col, row_offset, loc;

	col = sprite->x_pos / (N_BITS_IN_BYTE);
	row_offset = CalcRowOffset(sprite->y_pos, UINT8_N_COL);

	for (i = 0; i < sprite->bitmap.height; ++i) {
		loc = row_offset + col;
		frame_buffer[loc] = operation(frame_buffer[loc], sprite->bitmap.current_image[i]);
		row_offset += UINT8_N_COL;
	}
}

static inline UINT8 ClearOperation(UINT8 original, UINT8 image)
{
	return MASK_CLEAR;
}

static inline UINT8 AlphaOperation(UINT8 original, UINT8 image)
{
	return original ^ image;
}

static inline UINT8 DrawOperation(UINT8 original, UINT8 image)
{
	return original | image;
}

static inline void RasterClear(const UINT8 *frame_buffer, Sprite *sprite)
{
	CommonRasterOperation(frame_buffer, sprite, ClearOperation);
}

static inline void RasterAlpha(const UINT8 *frame_buffer, Sprite *sprite)
{
	CommonRasterOperation(frame_buffer, sprite, AlphaOperation);
}

static inline void RasterDraw(const UINT8 *frame_buffer, Sprite *sprite) 
{
	CommonRasterOperation(frame_buffer, sprite, DrawOperation);
}

static inline void PlotPoint(UINT8 *frame_buffer, int x, int y)
{
	int offset;

	offset = CalcOffset(y, x >> 3, UINT8_N_COL);
	frame_buffer[offset] |= MASK_SET;
}

static inline void Plot8Pixels(UINT8 *frame_buffer, int x, int y)
{
	int i, x_shifted, offset;

	x_shifted = x >> 3;

	for (i = 0; i < N_BITS_IN_BYTE; ++i) {
		offset = CalcOffset(y + i, x_shifted, UINT8_N_COL);
		frame_buffer[offset] |= MASK_SET;
	}
}

static inline void PlotLine(UINT8 *frame_buffer, int x0, int xf, int y0, int yf, int is_vertical) 
{
	int i, offset;

	if (is_vertical) {
		for (i = y0; i < yf; i += N_BITS_IN_BYTE) {
			Plot8Pixels(frame_buffer, x0, i);
		}
	} else {
		for (i = x0; i < xf; ++i) {
			offset = CalcOffset(y0, i >> 3, UINT8_N_COL);
			frame_buffer[offset] |= MASK_FILL;
		}
	}
}

static inline void PlotRectangle(UINT8 *frame_buffer, int length, int width, int x_pos, int y_pos)
{
	PlotLine(frame_buffer, x_pos, x_pos + width, y_pos, y_pos + length, 1);
	PlotLine(frame_buffer, x_pos, x_pos + width, y_pos + length, y_pos, 1);
	PlotLine(frame_buffer, x_pos, x_pos + width, y_pos, y_pos, 0);
	PlotLine(frame_buffer, x_pos + width, x_pos + width, y_pos, y_pos + length, 0);
}

static inline void PlotChar(UINT8 *frame_buffer, int x0, int y0, UINT8 bm)
{
	UINT8 *ch;
	int i, row_offset, loc;

	ch = IS_PRINTABLE(bm) ? GLYPH_START(bm) : NULL;
	if (!ch) return;

	row_offset = x0 / N_BITS_IN_BYTE;

	for (i = 0; i < FONT_HEIGHT; ++i) {
		loc = CalcOffset(y0 + i, row_offset, UINT8_N_COL);
		frame_buffer[loc] = ch[i];
	}
}

static inline void PrintScreen(UINT8 *frame_buffer, Sprite *sprite) 
{
	int row;

	for (row = 0; row < sprite->bitmap.height; ++row) {
		frame_buffer[row] |= sprite->bitmap.current_image[row];
	}
}

