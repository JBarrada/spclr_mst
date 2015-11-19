#include <text.h>
#include <stdint.h>
#include <stdbool.h>

#include <gfx.h>
#include <ui2.h>
#include <string.h>
#include "image/axis.h"
#include "image/axis_big.h"
#include "image/smallfont.h"

int16_t char_width(char c, enum TEXT_SIZE text_size) {
	switch (text_size) {
		case TS_BIG:
			return axis_big[c][0];
		case TS_DEFAULT:
			return axis[c][0];
		case TS_SMALL:
			return smallfont[c][0];
	}
}

int16_t char_height(enum TEXT_SIZE text_size) {
	switch (text_size) {
		case TS_BIG:
			return 25;
		case TS_DEFAULT:
			return 16;
		case TS_SMALL:
			return 8;
	}
}

int gfx_putch(int x, int y, char c, uint8_t color, enum TEXT_SIZE text_size) {
	uint8_t c_width = char_width(c, text_size);
	uint8_t c_height = char_height(text_size);

	uint8_t ix, iy, bpr;
	
	bpr = (c_width>>3) + ((c_width%8)?1:0);

	char* target;
	switch (text_size) {
		case TS_BIG:
			target = axis_big[c];
			break;
		case TS_DEFAULT:
			target = axis[c];
			break;
		case TS_SMALL:
			target = smallfont[c];
			break;
	}
	
	for (iy=0; iy<c_height; iy++) {
		for (ix=0; ix<c_width; ix++) {
			if ((target[1 + (bpr*iy) + (ix/8)] >> (ix%8)) & 1)
				set_pixel(x+ix, y+iy, color);
		}
	}
	
	return c_width;
}

void gfx_putstr(int x, int y, char *data, uint8_t color, enum TEXT_SIZE text_size) {
	int datalen = strlen(data);
	
	int i, c_width = 0;
	for (i=0; i<datalen; i++) {
		c_width = gfx_putch(x, y, data[i], color, text_size);
		x += c_width+1;
	}
}

void get_text_spread(char *data, enum TEXT_SIZE text_size, SIZE *size) {
	size->width = 0;
	size->height = char_height(text_size);
	
	int i, datalen=strlen(data);
	for (i=0; i<datalen; i++) {
		size->width += char_width(data[i], text_size) + 1;
	}
}
