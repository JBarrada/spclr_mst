#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <io.h>

char tbuf[32];
char bchars[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

void itoa_s(int i, uint8_t base, char* buf) {
	if (base > 16) return;
	if (i < 0) {
		*buf++ = '-';
		i *= -1;
	}
	itoa((uint32_t)i,base,buf);
}

void itoa(uint32_t i, uint8_t base, char* buf) {
	int pos = 0;
	int opos = 0;
	int top = 0;

	if (i == 0 || base > 16) {
		buf[0] = '0';
		buf[1] = '\0';
		return;
	}

	while (i != 0) {
		tbuf[pos] = bchars[i % base];
		pos++;
		i /= base;
	}
	top=pos--;
	for (opos=0; opos<top; pos--,opos++) {
		buf[opos] = tbuf[pos];
	}
	buf[opos] = 0;
}

unsigned int strlen(const char* str) {
	unsigned int ret = 0;
	while ( str[ret] != 0 )
		ret++;
	return ret;
}

unsigned int strlentrim(const char* str) {
	unsigned int ret = strlen(str);
	while (str[ret-1] == ' ' && (ret != 0))
		ret--;
	return ret;
}

void sstring(char *buf, char *formula, ...) {
	va_list args;
	int i, total_args=0;
	for (i=0; i<strlen(formula);i++) {
		if (formula[i]=='%')
			total_args++;
	}
	
	va_start(args, total_args);
	
	int str_pos=0;
	char itoa_buf[32], *string_buf;
	for (i=0; i<strlen(formula)-1;i++) {
		if (formula[i]=='%') {
			memcpy(buf+strlen(buf), formula+str_pos, i-str_pos);
			
			switch (formula[i+1]) {
				case 'd':
					memset(itoa_buf, 0, 32);
					itoa(va_arg(args, int), 10, itoa_buf);
					memcpy(buf+strlen(buf), itoa_buf, strlen(itoa_buf));
					break;
				case 'x':
					memset(itoa_buf, 0, 32);
					itoa(va_arg(args, int), 16, itoa_buf);
					memcpy(buf+strlen(buf), itoa_buf, strlen(itoa_buf));
					break;
				case 's':
					string_buf = va_arg(args, char*);
					memcpy(buf+strlen(buf), string_buf, strlen(string_buf));
					break;
				case 't':
					string_buf = va_arg(args, char*);
					memcpy(buf+strlen(buf), string_buf, strlentrim(string_buf));
					break;
			}
			str_pos = i+2;
		}
	}
	
	va_end(args);
	
	memcpy(buf+strlen(buf), formula+str_pos, strlen(formula)-str_pos);
}

void byte_swap(uint8_t *data, int len) {
	int i;
	uint8_t temp;
	for(i = 0; i < len; i += 2) {
		temp = data[i];
		data[i] = data[i+1];
		data[i+1] = temp;
	}
}