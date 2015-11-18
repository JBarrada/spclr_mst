#include <smart_self_test_page.h>
#include <dump_buf.h>
#include <sf_drive_list.h>
#include <ui.h>
#include <gfx.h>
#include <string.h>
#include <dump_buf.h>
#include <sf.h>
#include <stdint.h>
#include <io.h>
#include <keyboard.h>
#include <pit.h>
#include <ahci.h>
#include <ata.h>


void select_drives() { 
	
}

int self_test_short() {
	OS_DRIVE* selected_drive = sf_drive_list_page(0);
	if (selected_drive->is_active) {
		IDENTIFY_DEVICE_DATA id;
		//identify_device((HBA_PORT*)selected_drive->port, &id);
		SMART_DATA sd;
		//smart_read_data((HBA_PORT*)selected_drive->port, &sd);
		
		draw_top_bar(0, (CNTRLDISPLAY){0, 0, 0, 0, 1});
		
	}
}