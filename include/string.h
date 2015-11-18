#ifndef __STRING_H_
#define __STRING_H_

#include <stdint.h>
#include <stdarg.h>

void itoa_s(int i, uint8_t base, char* buf);
void itoa(uint32_t i, uint8_t base, char* buf);
unsigned int strlen(const char* str);
void sstring(char *buf, char *formula, ...);

void byte_swap(uint8_t *data, int len);

#endif