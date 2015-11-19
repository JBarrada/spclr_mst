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
//#include <ui.h>
#include <ui2.h>
#include <string.h>

#include <font.h>
#include <text.h>

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
	
	
	
	TEXT_UI tui3;
	init_text_ui(&tui3);
	tui3.text = "test3!";
	
	TEXT_UI tui4;
	init_text_ui(&tui4);
	tui4.text = "test4!";
	tui4.layout.margin.left = 50;
	
	CONTAINER_UI cui2;
	init_container_ui(&cui2);
	cui2.content[0] = (uint32_t)&tui3;
	cui2.content[1] = (uint32_t)&tui4;
	cui2.layout.margin.top = 50;
	
	
	
	TEXT_UI tui;
	init_text_ui(&tui);
	tui.text = "test!";
	
	TEXT_UI tui2;
	init_text_ui(&tui2);
	tui2.text = "test2!";
	tui2.layout.margin.left = 50;

	CONTAINER_UI cui;
	init_container_ui(&cui);
	cui.content[0] = (uint32_t)&tui;
	cui.content[1] = (uint32_t)&tui2;
	cui.content[2] = (uint32_t)&cui2;
	
	cui.format.fg = 1;
	cui.format.bg = 3;
	
	cui.layout.padding = (THICKNESS){5,5,5,5};
	
	cui.layout.layout_align_h = A_RIGHT;
	cui.layout.layout_align_v = A_BOTTOM;
	
	
	cui.draw(&cui);
	//tui.draw(&tui2);

	swap_buffers();
	
	//pci_probe();
	
	//main_menu_loop();

	for(;;) {
		asm("hlt");
	}
}
