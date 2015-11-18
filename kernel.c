#define KERNEL

#include <os.h>
#include <stddef.h>
#include <stdint.h>

#include <keyboard.h>
#include <io.h>

#include <gdt.h>
#include <idt.h>
#include <pic.h>
#include <pit.h>
#include <pci.h>
#include <ahci.h>

#include <vga.h>
#include <gfx.h>
#include <ui.h>
#include <string.h>

#include <font.h>

#include <main_menu.h>

void kernel_main() {
	vga_init();
	gfx_init();
	
	gdt_init();
	idt_init();
	pic_init();
	
	pit_init();
	keyboard_init();
	pci_init();
	
	asm("sti");
	
	pci_probe();
	
	main_menu_loop();

	for(;;) {
		asm("hlt");
	}
}
