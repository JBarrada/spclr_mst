#ifndef __FONT_H_
#define __FONT_H_

int putch(int x, int y, char c, unsigned char color);
void putstr(int x, int y, char *data, unsigned char color);
int get_str_width(char *data);

int putch_big(int x, int y, char c, unsigned char color);
void putstr_big(int x, int y, char *data, unsigned char color);
int get_str_width_big(char *data);

int putch_small(int x, int y, char c, unsigned char color);
void putstr_small(int x, int y, char *data, unsigned char color);

#endif