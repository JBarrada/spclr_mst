#include <sf_drive_list.h>
#include <sf.h>
#include <os.h>
#include <stdint.h>
#include <pci.h>
#include <ahci.h>
#include <gfx.h>
#include <ui.h>
#include <keyboard.h>
#include <ata.h>
#include <string.h>
#include <io.h>

LIST sf_drive_list;

void setup_sf_drive_list_container(OS_HBA *hba) {
	/*
	memset(&sf_drive_list, 0, sizeof(LIST));
	sf_drive_list.size.width = 600;
	sf_drive_list.bg = 4;
	sf_drive_list.vpad = sf_drive_list.hpad = 10;

	ITEM *sf_drive_list_header = &sf_drive_list.header;
	sf_drive_list_header->size.width = 600;
	sf_drive_list_header->size.height = 15;
	sf_drive_list_header->bg = 4;
	sf_drive_list_header->active = 1;
	sf_drive_list_header->fg = -1;
	
	setup_text(&sf_drive_list_header->header_texts[0], "Interface: AHCI", (POSITION){4,2}, 2, align_left);
	setup_text(&sf_drive_list_header->header_texts[1], "", (POSITION){4,2}, 3, align_right);
	
	sstring(sf_drive_list_header->header_texts[1].text1, "(B%d:S%d:F%d) V:%x D:%x", 
		hba->bus, hba->slot, hba->function, hba->vendorid, hba->deviceid);
		*/
}

void setup_sf_drive_list_item(OS_DRIVE *drive, ITEM *sf_drive_item) {
	/*
	sf_drive_item->size.width = 590;
	sf_drive_item->size.height = 20;
	sf_drive_item->selectedsize.height = 120;
	sf_drive_item->bg = 3;
	sf_drive_item->selectedbg = 1;
	sf_drive_item->fg = 1;
	sf_drive_item->selectedfg = 3;
	sf_drive_item->active = 1;
	
	drive->refresh_identify(drive);
	
	setup_text(&sf_drive_item->header_texts[0], "", (POSITION){4,1}, 1, align_left);
	setup_text(&sf_drive_item->header_texts[1], "", (POSITION){4,1}, 1, align_right);
	sstring(sf_drive_item->header_texts[0].text1, "%t", drive->id.model);
	sstring(sf_drive_item->header_texts[1].text1, "PORT %d", drive->port_num);
	
	setup_text_list_item(&sf_drive_item->body_texts[0], "Serial Number:", "", (POSITION){8,20}, 4, align_left);
	setup_text_list_item(&sf_drive_item->body_texts[1], "Firmware Revision:", "", (POSITION){8,36}, 4, align_left);
	setup_text_list_item(&sf_drive_item->body_texts[2], "Capacity:", "", (POSITION){8,52}, 4, align_left);
	setup_text_list_item(&sf_drive_item->body_texts[3], "Security:", "", (POSITION){8,100}, 4, align_left);
	sstring(sf_drive_item->body_texts[0].text2, "%t", drive->id.serial);
	sstring(sf_drive_item->body_texts[1].text2, "%t", drive->id.firmware);
	sstring(sf_drive_item->body_texts[2].text2, "%dGB", (uint32_t)((drive->id.max_lba*512)>>30));
	
	if (drive->id.sec_supported) {
		uint8_t sec = (drive->id.sec_enabled << 2) | (drive->id.sec_locked << 1) | drive->id.sec_frozen;
		char sec_name[][10] = {"SEC1", "SEC2", "", "", "SEC5", "SEC6", "SEC4"};
		sstring(sf_drive_item->body_texts[3].text2, "%s", sec_name[sec]);
	}
	else 
		sstring(sf_drive_item->body_texts[3].text2, "%s", "Not Supported");
	*/
}

OS_DRIVE* sf_drive_list_page(unsigned int selected) {
	/*
	broadcast_msg("Refreshing HBAs...", 1, 0, 1+8, 3+8);

	hbas[0].refresh_hba(&hbas[0]);

	if (hbas[0].is_active) {
		setup_sf_drive_list_container(&hbas[0]);
		
		int num_devices = 0;
		int match[32];
		//uint8_t diag_key[16] = {1,1,5,1};
		int i;
		for (i=0; i<32; i++) {
			//if (diag_unlock((HBA_PORT*)ahci_dev.drives[i].port, diag_key)) {
				if (hbas[0].drives[i].is_active) {
					setup_sf_drive_list_item(&hbas[0].drives[i], &sf_drive_list.items[num_devices]);
					match[num_devices] = i;
					if (num_devices==selected)
						sf_drive_list.items[num_devices].selected = 1;
					num_devices++;
				}
			//}
		}
		ashes_palette();
		
		if (num_devices == 0) {
			broadcast_msg("Unable to find supported drives...", 1, 1, 1+8, 3+8);
			return (OS_DRIVE*)0;
		}
			
		
		uint16_t key;
		while (1) {
			draw_top_bar(0, (CNTRLDISPLAY){1, 1, 1, 0, 1});
			draw_list(&sf_drive_list, 40, 40);
			swap_buffers();
			
			key = get_key();
			switch (key&0xff) {
				case DOWN_PRESSED:
				case RIGHT_PRESSED:
					selected++; break;
				case UP_PRESSED:
				case LEFT_PRESSED:
					selected--; break;
					
				case ENTER_PRESSED:
					return &hbas[0].drives[match[selected]];
				case F5_PRESSED:
					return sf_drive_list_page(selected);
				case ESC_PRESSED:
					return (OS_DRIVE*)0;

				default:
					continue;
			}
			
			selected = (selected<0)?num_devices-1:selected;
			selected %= num_devices;
			for (i=0;i<num_devices;i++) 
				sf_drive_list.items[i].selected = (i==selected);
		}
	}
	
	*/
}