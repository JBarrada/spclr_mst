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

void menushit_select(TEXT_UI *ui_element) {
	ui_element->format.bg = 0;
}
void menushit_deselect(TEXT_UI *ui_element) {
	ui_element->format.bg = 4;
}

bool progress_key(PROGRESS_UI *ui_element, uint16_t key) {
	switch(key) {
		case 0x0c:
			ui_element->progress -= 1; return true;
		case 0x0d:
			ui_element->progress += 1; return true;
	}
	return false;
}

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
	tui3.select = menushit_select;
	
	TEXT_UI tui4;
	init_text_ui(&tui4);
	tui4.text = "test4!";
	tui4.format.bg = 3;
	tui4.select = menushit_select;
	tui4.layout.margin.left = 80;
	tui4.layout.layout_align_v = A_STRETCH;
	tui4.layout.content_align_v = A_CENTER;
	
	CONTAINER_UI cui2;
	init_container_ui(&cui2);
	cui2.content[0] = (uint32_t)&tui3;
	cui2.content[1] = (uint32_t)&tui4;
	cui2.layout.height = 100;
	cui2.layout.h_auto = false;
	cui2.layout.padding = (THICKNESS){5,5,5,5};
	
	
	TEXT_UI tui;
	init_text_ui(&tui);
	tui.text = "test1!";
	tui.format.bg = 4;
	tui.select = menushit_select;
	tui.deselect = menushit_deselect;
	
	TEXT_UI tui2;
	init_text_ui(&tui2);
	tui2.text = "test2!";
	tui2.format.bg = 4;
	tui2.layout.layout_align_h = A_STRETCH;
	tui2.layout.content_align_h = A_RIGHT;
	tui2.select = menushit_select;
	tui2.deselect = menushit_deselect;
	
	CHECKBOX_UI chui;
	init_checkbox_ui(&chui);
	
	PROGRESS_UI pui;
	init_progress_ui(&pui);
	pui.progress = 10;
	pui._key_press = progress_key;
	
	LIST_UI lui;
	init_list_ui(&lui);
	lui.format.bg = 3;
	lui.layout.width = 400;
	lui.layout.w_auto = false;
	lui.layout.layout_align_h = A_CENTER;
	lui.layout.layout_align_v = A_CENTER;
	
	lui.content[0] = (uint32_t)&tui;
	lui.content[1] = (uint32_t)&tui2;
	lui.content[2] = (uint32_t)&chui;
	lui.content[3] = (uint32_t)&pui;
	//lui.content[2] = (uint32_t)&cui2;
	
	
	lui.draw(&lui);
	swap_buffers();
	
	uint16_t key = 0;
	while (1) {
		key = get_key();
		lui._key_press(&lui, key);
		//broadcast_msg("done", 1, 1, 1, 0);
		lui.draw(&lui);
		swap_buffers();
	}

	//pci_probe();
	
	//main_menu_loop();

	for(;;) {
		asm("hlt");
	}
}
