#include <vga.h>
#include <io.h>

unsigned char g_640x480x16[] = {
/* MISC */
	0xE3,
/* SEQ */
	0x03, 0x01, 0x08, 0x00, 0x06,
/* CRTC */
	0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0B, 0x3E,
	0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xEA, 0x0C, 0xDF, 0x28, 0x00, 0xE7, 0x04, 0xE3,
	0xFF,
/* GC */
	0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x05, 0x0F,
	0xFF,
/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x01, 0x00, 0x0F, 0x00, 0x00
};

unsigned char* video_buffer = (unsigned char*) 0xA0000;

void write_regs(unsigned char *regs) {
	unsigned i;

	// write MISCELLANEOUS reg
	outb(VGA_MISC_WRITE, *regs);
	regs++;
	
	// write SEQUENCER regs
	for(i = 0; i < VGA_NUM_SEQ_REGS; i++) {
		outb(VGA_SEQ_INDEX, i);
		outb(VGA_SEQ_DATA, *regs);
		regs++;
	}
	
	// unlock CRTC registers
	outb(VGA_CRTC_INDEX, 0x03);
	outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) | 0x80);
	outb(VGA_CRTC_INDEX, 0x11);
	outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) & ~0x80);
	
	// make sure they remain unlocked
	regs[0x03] |= 0x80;
	regs[0x11] &= ~0x80;
	
	// write CRTC regs
	for(i = 0; i < VGA_NUM_CRTC_REGS; i++) {
		outb(VGA_CRTC_INDEX, i);
		outb(VGA_CRTC_DATA, *regs);
		regs++;
	}
	
	// write GRAPHICS CONTROLLER regs
	for(i = 0; i < VGA_NUM_GC_REGS; i++) {
		outb(VGA_GC_INDEX, i);
		outb(VGA_GC_DATA, *regs);
		regs++;
	}
	
	// write ATTRIBUTE CONTROLLER regs
	for(i = 0; i < VGA_NUM_AC_REGS; i++) {
		(void)inb(VGA_INSTAT_READ);
		outb(VGA_AC_INDEX, i);
		outb(VGA_AC_WRITE, *regs);
		regs++;
	}
	
	// lock 16-color palette and unblank display
	(void)inb(VGA_INSTAT_READ);
	outb(VGA_AC_INDEX, 0x20);
}

void set_plane(unsigned p) {
	unsigned char pmask;

	p &= 3;
	pmask = 1 << p;
	
	// set read plane
	outb(VGA_GC_INDEX, 4);
	outb(VGA_GC_DATA, p);
	
	// set write plane
	outb(VGA_SEQ_INDEX, 2);
	outb(VGA_SEQ_DATA, pmask);
}

void set_palette_color(unsigned char color, unsigned char r, unsigned char g, unsigned char b) {
	outb(0x03C6,0xff);
	outb(0x03C8,color);
	outb(0x03C9,r);
	outb(0x03C9,g);
	outb(0x03C9,b);
}

void setup_palette(unsigned int *pal) {
	unsigned int color_index[16] = { 0,  1,  2,  3,  4,  5, 20, 7, 
									56, 57, 58, 59, 60, 61, 62, 63};	
	int i;
	for (i=0; i<16; i++) {
		set_palette_color(color_index[i], ((pal[i]>>16)&0xff)/4, ((pal[i]>>8)&0xff)/4, ((pal[i]>>0)&0xff)/4);
	}
}

void write_pixel_row(unsigned int y, unsigned char *d) {
	unsigned int p, i, di;
	char plane_bits[80];
	int off = 80 * y;
	for(p = 0; p < 4; p++) {
		set_plane(p);
		for (i=0; i<80; i++) {
			di = i*8;
			plane_bits[i] = ((d[di  ] >> p) & 1) << 7 | ((d[di+1] >> p) & 1) << 6 | ((d[di+2] >> p) & 1) << 5 | ((d[di+3] >> p) & 1) << 4 |
							((d[di+4] >> p) & 1) << 3 | ((d[di+5] >> p) & 1) << 2 | ((d[di+6] >> p) & 1) << 1 | ((d[di+7] >> p) & 1);
		}
		memcpy(video_buffer+off, plane_bits, 80);
	}
}

void vga_init() {
	write_regs(g_640x480x16);
}