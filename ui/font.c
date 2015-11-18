#include <gfx.h>
#include <font.h>
#include <string.h>
#include "image/axis.h"
#include "image/axis_big.h"
#include "image/smallfont.h"


int putch(int x, int y, char c, unsigned char color) {
	int char_width = axis[c][0];
	int char_height = 16;
	
	int ix, iy, bpr;
	unsigned char bit;
	
	bpr = (char_width/8);
	bpr += (char_width%8)?1:0;
	
	for (iy=0; iy<char_height; iy++) {
		for (ix=0; ix<char_width; ix++) {
			bit = axis[c][1 + (bpr*iy) + (ix/8)] >> (ix%8);
			bit &= 1;
			
			if (bit) {
				set_pixel(x+ix, y+iy, color);
			}
		}
	}
	
	return char_width;
}

void putstr(int x, int y, char *data, unsigned char color) {
	int datalen = strlen(data);
	
	int char_width = 0;
	for (int i = 0; i < datalen; i++) {
		char_width = putch(x, y, data[i], color);
		x += char_width+1;
	}
}

int putch_big(int x, int y, char c, unsigned char color) {
	int char_width = axis_big[c][0];
	int char_height = 25;
	
	int ix, iy, bpr;
	unsigned char bit;
	
	bpr = (char_width/8);
	bpr += (char_width%8)?1:0;
	
	for (iy=0; iy<char_height; iy++) {
		for (ix=0; ix<char_width; ix++) {
			bit = axis_big[c][1 + (bpr*iy) + (ix/8)] >> (ix%8);
			bit &= 1;
			
			if (bit) {
				set_pixel(x+ix, y+iy, color);
			}
		}
	}
	
	return char_width;
}

void putstr_big(int x, int y, char *data, unsigned char color) {
	int datalen = strlen(data);
	
	int char_width = 0;
	for (int i = 0; i < datalen; i++) {
		char_width = putch_big(x, y, data[i], color);
		x += char_width+2;
	}
}

int putch_small(int x, int y, char c, unsigned char color) {
	int char_width = smallfont[c][0];
	int char_height = 8;
	
	int ix, iy, bpr;
	unsigned char bit;
	
	bpr = (char_width/8);
	bpr += (char_width%8)?1:0;
	
	for (iy=0; iy<char_height; iy++) {
		for (ix=0; ix<char_width; ix++) {
			bit = smallfont[c][1 + (bpr*iy) + (ix/8)] >> (ix%8);
			bit &= 1;
			
			if (bit) {
				set_pixel(x+ix, y+iy, color);
			}
		}
	}
	
	return char_width;
}

void putstr_small(int x, int y, char *data, unsigned char color) {
	int datalen = strlen(data);
	
	int char_width = 0;
	for (int i = 0; i < datalen; i++) {
		char_width = putch_small(x, y, data[i], color);
		x += char_width+1;
	}
}


int get_str_width(char *data) {
	int str_width=0, datalen=strlen(data);
	
	for (int i = 0; i < datalen; i++) {
		str_width += axis[data[i]][0] + 1;
	}
	return str_width;
}

int get_str_width_big(char *data) {
	int str_width=0, datalen=strlen(data);
	
	for (int i = 0; i < datalen; i++) {
		str_width += axis_big[data[i]][0] + 2;
	}
	return str_width;
}