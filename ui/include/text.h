#ifndef __TEXT_H_
#define __TEXT_H_

#include <ui2.h>
#include <stdint.h>
#include <stdbool.h>

int gfx_putch(int x, int y, char c, uint8_t color, enum TEXT_SIZE text_size);
void gfx_putstr(int x, int y, char *data, uint8_t color, enum TEXT_SIZE text_size);
void get_text_spread(char *data, enum TEXT_SIZE text_size, SIZE *size);

#endif