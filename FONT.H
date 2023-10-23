/* Definitions and macros for system font and character handling */
#ifndef FONT_H
#define FONT_H

#include "types.h"

extern const UINT8 font[];     /* 8x8 system font */

#define CHAR_BEL   7
#define CHAR_TAB   9
#define CHAR_LF   10
#define CHAR_VT   11
#define CHAR_FF   12
#define CHAR_CR   13
#define CHAR_DEL 127

#define IS_PRINTABLE(i) ((i) >= 32 && (i) <= 126)
#define GLYPH_START(i) (font + ((i - 32) << 3))

#endif
