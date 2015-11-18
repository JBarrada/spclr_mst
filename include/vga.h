#ifndef __VGA_H_
#define __VGA_H_

#define	VGA_AC_INDEX		0x3C0
#define	VGA_AC_WRITE		0x3C0
#define	VGA_MISC_WRITE		0x3C2
#define VGA_SEQ_INDEX		0x3C4
#define VGA_SEQ_DATA		0x3C5
#define VGA_GC_INDEX 		0x3CE
#define VGA_GC_DATA 		0x3CF

#define VGA_CRTC_INDEX		0x3D4
#define VGA_CRTC_DATA		0x3D5
#define	VGA_INSTAT_READ		0x3DA

#define	VGA_NUM_SEQ_REGS	5
#define	VGA_NUM_CRTC_REGS	25
#define	VGA_NUM_GC_REGS		9
#define	VGA_NUM_AC_REGS		21

void write_regs(unsigned char *regs);
void set_plane(unsigned p);
void set_palette_color(unsigned char color, unsigned char r, unsigned char g, unsigned char b);
void setup_palette(unsigned int *pal);
void write_pixel_row(unsigned int y, unsigned char *d);

void vga_init();

#endif