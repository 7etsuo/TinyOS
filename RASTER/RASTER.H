#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "../TYPES.H"

#define SCREEN_WIDTH_NBITS 640
#define SCREEN_HEIGHT_NBITS 400
#define N_BITS_IN_BYTE 8
#define NUM_FRAME_BUFFERS 2
#define NUM_SPRITE_STATES 3
#define FONT_HEIGHT 16
#define UINT8_N_COL 80
#define UINT16_N_COL 40
#define UINT32_N_COL 20
#define MASK_SET 0x80
#define MASK_CLEAR 0x00
#define MASK_FILL 0xff

/**
 * @brief Sprite structure definition.
 */
typedef struct Sprite Sprite;
typedef void (*RasterFunction)(const UINT8 *, Sprite *);

/**
 * @brief Raster functions structure.
 */
typedef struct {
    RasterFunction Draw;
    RasterFunction Alpha;
    RasterFunction Clear;
} RasterFunctions;

/**
 * @brief Bitmap structure.
 */
typedef struct {
    int height;
    UINT8 *stored_images[NUM_SPRITE_STATES];
    UINT8 *current_image;
    RasterFunctions raster;
} Bitmap;

/**
 * @brief Calculates the row offset in the frame buffer.
 * @param y Y-coordinate.
 * @param col_width Width of a column.
 * @return Calculated row offset.
 */
static inline int CalcRowOffset(int y, int col_width);

/**
 * @brief Calculates the offset in the frame buffer.
 * @param row Row index.
 * @param col Column index.
 * @param col_width Width of a column.
 * @return Calculated offset.
 */
static inline int CalcOffset(int row, int col, int col_width);

/**
 * @brief Performs common raster operations.
 * @param frame_buffer Frame buffer pointer.
 * @param sprite Pointer to the sprite structure.
 * @param operation Function pointer to the raster operation.
 */
static inline void CommonRasterOperation(const UINT8 *frame_buffer, Sprite *sprite, UINT8 (*operation)(UINT8, UINT8));

/**
 * @brief Clears a raster.
 * @param original Original frame buffer data.
 * @param image Image to overlay.
 * @return Cleared raster data.
 */
static inline UINT8 ClearOperation(UINT8 original, UINT8 image);

/**
 * @brief Alpha blending operation.
 * @param original Original frame buffer data.
 * @param image Image to overlay.
 * @return Alpha-blended raster data.
 */
static inline UINT8 AlphaOperation(UINT8 original, UINT8 image);

/**
 * @brief Draws a raster.
 * @param original Original frame buffer data.
 * @param image Image to overlay.
 * @return Drawn raster data.
 */
static inline UINT8 DrawOperation(UINT8 original, UINT8 image);

/**
 * @brief Plots a point on the frame buffer.
 * @param frame_buffer Frame buffer pointer.
 * @param x X-coordinate.
 * @param y Y-coordinate.
 */
static inline void PlotPoint(UINT8 *frame_buffer, int x, int y);

/**
 * @brief Plots 8 pixels on the frame buffer.
 * @param frame_buffer Frame buffer pointer.
 * @param x X-coordinate.
 * @param y Y-coordinate.
 */
static inline void Plot8Pixels(UINT8 *frame_buffer, int x, int y);

/**
 * @brief Plots a line on the frame buffer.
 * @param frame_buffer Frame buffer pointer.
 * @param x0 Initial x-coordinate.
 * @param xf Final x-coordinate.
 * @param y0 Initial y-coordinate.
 * @param yf Final y-coordinate.
 * @param is_vertical Flag indicating if the line is vertical.
 */
static inline void PlotLine(UINT8 *frame_buffer, int x0, int xf, int y0, int yf, int is_vertical);

/**
 * @brief Plots a rectangle on the frame buffer.
 * @param frame_buffer Frame buffer pointer.
 * @param length Length of the rectangle.
 * @param width Width of the rectangle.
 * @param x_pos X-coordinate of the rectangle.
 * @param y_pos Y-coordinate of the rectangle.
 */
static inline void PlotRectangle(UINT8 *frame_buffer, int length, int width, int x_pos, int y_pos);

/**
 * @brief Plots a character on the frame buffer.
 * @param frame_buffer Frame buffer pointer.
 * @param x0 X-coordinate of the character.
 * @param y0 Y-coordinate of the character.
 * @param bm Bitmap of the character.
 */
static inline void PlotChar(UINT8 *frame_buffer, int x0, int y0, UINT8 bm);

/**
 * @brief Prints a sprite on the frame buffer.
 * @param frame_buffer Frame buffer pointer.
 * @param sprite Pointer to the sprite structure.
 */
static inline void PrintScreen(UINT8 *frame_buffer, Sprite *sprite);

#endif // GRAPHICS_H

