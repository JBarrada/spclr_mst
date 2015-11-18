#include <main_menu.h>
#include <os.h>
#include <dump_buf.h>
#include <sf_drive_list.h>
#include <ui.h>
#include <string.h>
#include <dump_buf.h>
#include <sf.h>
#include <stdint.h>
#include <io.h>
#include <keyboard.h>
#include <pit.h>
#include <ahci.h>

MENU main_menu;

void main_menu_loop() {
	char main_menu_items[6][100] = {"Security Erase", "SMART Self-Test Short", "SMART Self-Test Extended", "Clear SMART", "About", "Reboot"};
	
	int main_menu_selected = 0;
	while (1) {
		main_menu_selected = menu_page(main_menu, main_menu_items, 6, (CNTRLDISPLAY){1, 1, 0, 0 ,0}, 4, main_menu_selected);
		
		if (main_menu_selected == 0) {
			OS_DRIVE* selected_drive = sf_drive_list_page(0);
			if (selected_drive->is_active) {
				int result = secure_erase((HBA_PORT*)selected_drive->port);
			}
		}
		
		if (main_menu_selected == 4) {
			broadcast_msg("'SPCLR' is the result of caffeine + Justin Barrada", 0, 1, 1+8, 0+8);
		}
		if (main_menu_selected == 5) {
			uint8_t good = 0x02;
			while (good & 0x02)
				good = inb(0x64);
			outb(0x64, 0xFE);
			//halt();
		}
	}
}