#ifndef __GFX_H_
#define __GFX_H_

#define WIDTH_W 640
#define HEIGHT_W 480

typedef struct tagPOINT {
	double x, y;
} POINT;

void swap_buffers();
void ashes_palette();
void ashes_palette_dim();
void set_pixel(int x, int y, unsigned char c);
void line(int x1, int y1, int x2, int y2, unsigned char c);
void circle_filled(int cx, int cy, int r, unsigned char c);
void poly_filled(POINT points[], int num_points, unsigned char c);
void rectangle_filled(int x, int y, int width, int height, unsigned char c);
void draw_glyph(int x, int y, int width, int height, char *data, int mask_color, int invert);

void capture();
void restore();

void gfx_init();

#endif