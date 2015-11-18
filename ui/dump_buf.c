#include <dump_buf.h>
#include <pci.h>
#include <ahci.h>
#include <gfx.h>
#include <ui.h>
#include <keyboard.h>
#include <string.h>
#include <io.h>
#include <stdint.h>
#include <font.h>

void dump_buffer(uint8_t *buffer, int count) {
	draw_top_bar(0, (CNTRLDISPLAY){0, 0, 0, 0, 1});
	int i, xoffset=4, yoffset=34;
	char hex_buffer[10];
	for (i=0; i<count; i++) {
		memset(hex_buffer, 0, 10);
		sstring(hex_buffer, "%x", buffer[i]);
		putstr_small(xoffset, yoffset, hex_buffer, 1);
		xoffset += 16;
		
		if ((i+1)%16==0) {
			yoffset += 8;
			xoffset = 4;
		}
	}
	swap_buffers();
	uint16_t key;
	while (1) {
		key = get_key();
		switch (key) {
			case ESC_PRESSED:
				return;
		}
	}
}