#include <sf_drive_list.h>
#include <sf.h>
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

void setup_sf_drive_list_container(AHCI_DEV_UI *ahci_dev) {
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
		ahci_dev->bus, ahci_dev->slot, ahci_dev->function, ahci_dev->vendorid, ahci_dev->deviceid);
}

void setup_sf_drive_list_item(DRIVE_UI *drive, ITEM *sf_drive_item) {
	sf_drive_item->size.width = 590;
	sf_drive_item->size.height = 20;
	sf_drive_item->selectedsize.height = 120;
	sf_drive_item->bg = 3;
	sf_drive_item->selectedbg = 1;
	sf_drive_item->fg = 1;
	sf_drive_item->selectedfg = 3;
	sf_drive_item->active = 1;
	
	// identify data
	IDENTIFY_DEVICE_DATA id;
	identify_device((HBA_PORT*)drive->port, &id);
	
	setup_text(&sf_drive_item->header_texts[0], "", (POSITION){4,1}, 1, align_left);
	setup_text(&sf_drive_item->header_texts[1], "", (POSITION){4,1}, 1, align_right);
	sstring(sf_drive_item->header_texts[0].text1, "%t", id.model);
	sstring(sf_drive_item->header_texts[1].text1, "PORT %d", drive->port_num);
	
	setup_text_list_item(&sf_drive_item->body_texts[0], "Serial Number:", "", (POSITION){8,20}, 4, align_left);
	setup_text_list_item(&sf_drive_item->body_texts[1], "Firmware Revision:", "", (POSITION){8,36}, 4, align_left);
	setup_text_list_item(&sf_drive_item->body_texts[2], "Capacity:", "", (POSITION){8,52}, 4, align_left);
	setup_text_list_item(&sf_drive_item->body_texts[3], "Security:", "", (POSITION){8,100}, 4, align_left);
	sstring(sf_drive_item->body_texts[0].text2, "%t", id.serial);
	sstring(sf_drive_item->body_texts[1].text2, "%t", id.firmware);
	sstring(sf_drive_item->body_texts[2].text2, "%dGB", (uint32_t)((id.max_lba*512)>>30));
	
	if (id.sec_supported) {
		// SEC1
		if ((id.sec_enabled == 0) && (id.sec_locked == 0) && (id.sec_frozen == 0))
			sstring(sf_drive_item->body_texts[3].text2, "%s", "SEC1");
		// SEC2
		if ((id.sec_enabled == 0) && (id.sec_locked == 0) && (id.sec_frozen == 1))
			sstring(sf_drive_item->body_texts[3].text2, "%s", "SEC2");
		// SEC4
		if ((id.sec_enabled == 1) && (id.sec_locked == 1) && (id.sec_frozen == 0))
			sstring(sf_drive_item->body_texts[3].text2, "%s", "SEC4");
		// SEC5
		if ((id.sec_enabled == 1) && (id.sec_locked == 0) && (id.sec_frozen == 0))
			sstring(sf_drive_item->body_texts[3].text2, "%s", "SEC5");
		// SEC6
		if ((id.sec_enabled == 1) && (id.sec_locked == 0) && (id.sec_frozen == 1))
			sstring(sf_drive_item->body_texts[3].text2, "%s", "SEC2");
	}
	else 
		sstring(sf_drive_item->body_texts[3].text2, "%s", "Not Supported");

	/*
	// diaginfo data
	DIAG_INFO di;
	get_diag_info((HBA_PORT*)drive->port, &di);
	
	setup_text_list_item(&sf_drive_item->body_texts[4], "Customer ID:", "", (POSITION){8,68}, 4, align_left);
	setup_text_list_item(&sf_drive_item->body_texts[5], "Config ID:", "", (POSITION){8,84}, 4, align_left);
	sstring(sf_drive_item->body_texts[4].text2, "%d", (uint16_t)di.cust_id[0] | (uint16_t)(di.cust_id[1]<<8));
	sstring(sf_drive_item->body_texts[5].text2, "%t", di.config_id);
	*/
}

DRIVE_UI sf_drive_list_page(unsigned int selected) {
	broadcast_msg("Probing for AHCI devices...", 1, 0, 1+8, 3+8);
	
	AHCI_DEV_UI ahci_dev;
	memset(&ahci_dev, 0, sizeof(AHCI_DEV_UI));
	pci_probe_for_ahci(&ahci_dev);
	
	if (ahci_dev.ABAR) {
		setup_sf_drive_list_container(&ahci_dev);
		
		int num_devices = probe_ahci2(&ahci_dev);
		
		uint8_t diag_key[16] = {1,1,5,1};
		int i, sf_drive_item_num = 0;
		for (i=0; i<num_devices; i++) {
			//if (diag_unlock((HBA_PORT*)ahci_dev.drives[i].port, diag_key)) {
				setup_sf_drive_list_item(&ahci_dev.drives[i], &sf_drive_list.items[sf_drive_item_num]);
				if (sf_drive_item_num==selected)
					sf_drive_list.items[sf_drive_item_num].selected = 1;
				sf_drive_item_num++;
			//}
		}
		ashes_palette();
		
		if (sf_drive_item_num == 0) {
			broadcast_msg("Unable to find supported drives...", 1, 1, 1+8, 3+8);
			return (DRIVE_UI){0,0,0};
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
					return ahci_dev.drives[selected];
				case F5_PRESSED:
					return sf_drive_list_page(selected);
				case ESC_PRESSED:
					return (DRIVE_UI){0,0,0};

				default:
					continue;
			}
			
			selected %= sf_drive_item_num;
			selected = (selected<0)?sf_drive_item_num-1:selected;
			for (i=0;i<sf_drive_item_num;i++) 
				sf_drive_list.items[i].selected = (i==selected);
		}
	}
}