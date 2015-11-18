#include <sf.h>
#include <stdint.h>
#include <ahci.h>
#include <aes.h>
#include <io.h>
#include <ata.h>
#include <ui.h>
#include <string.h>


int get_diag_info(HBA_PORT *port, DIAG_INFO *di) {
	uint8_t smart_write_log[8] = {0xd6, 0x01, 0xe0, 0x4f, 0xc2, 0x00, 0xb0, 0x00};
	uint8_t smart_read_log[8] = {0xd5, 0x01, 0xe1, 0x4f, 0xc2, 0x00, 0xb0, 0x00};
	uint8_t write_buf[512] = {0x00, 0xc0};
	uint8_t read_buf[512];
	if (ata_command(port, smart_write_log, write_buf, 512, WRITE_TO_DEV)) {
		if (ata_command(port, smart_read_log, read_buf, 512, READ_FROM_DEV)) {
			memcpy(di->diag_revision, read_buf, 4);
			memcpy(di->access_level, read_buf+4, 2);
			memcpy(di->oem_id, read_buf+8, 4);
			memcpy(di->cust_id, read_buf+12, 4);
			memcpy(di->config_id, read_buf+16, 8);
			memcpy(di->firmware_rev, read_buf+24, 16);
			memcpy(di->hardware_rev, read_buf+40, 16);
			memcpy(di->diag_challenge, read_buf+256, 256);
			
			return 1;
		}
	}
	return 0;
}

int is_diag_unlocked(HBA_PORT *port) {
	DIAG_INFO di;
	get_diag_info(port, &di);
	if (di.access_level[0] == 1)
		return 1;
	else
		return 0;
}

int diag_unlock(HBA_PORT *port, uint8_t *key) {
	DIAG_INFO di;
	if (get_diag_info(port, &di)) {
		if (di.access_level[0] == 0) {		
			uint8_t iv[] = {0xFA, 0x8D, 0x46, 0x54, 0x83, 0xC6, 0xED, 0xD8, 0x37, 0x5A, 0x9D, 0xC1, 0x3E, 0x69, 0x1B, 0x04};
			uint8_t encrypted[256];
			AES128_CBC_encrypt_buffer(encrypted, di.diag_challenge, 256, key, iv);
			
			uint8_t unlock_buf[512] = {0x01, 0xc0, 0x01};
			memcpy(unlock_buf+256, encrypted, 256);
			uint8_t smart_write_log[8] = {0xd6, 0x01, 0xe0, 0x4f, 0xc2, 0x00, 0xb0, 0x00};
			if (ata_command(port, smart_write_log, unlock_buf, 512, WRITE_TO_DEV))
				return is_diag_unlocked(port);
			else
				return 0;
		}
		return 1;
	}
	return 0;
}

int read_cdu(HBA_PORT *port, uint8_t *cdu) {
	uint8_t smart_write_log[8] = {0xd6, 0x01, 0xe0, 0x4f, 0xc2, 0x00, 0xb0, 0x00};
	uint8_t smart_read_log[8] = {0xd5, 0x07, 0xe1, 0x4f, 0xc2, 0x00, 0xb0, 0x00};
	uint8_t write_buf[512] = {0x08, 0xc0};
	if (ata_command(port, smart_write_log, write_buf, 512, WRITE_TO_DEV))
		if (ata_command(port, smart_read_log, cdu, 3584, READ_FROM_DEV))
			return 1;
	return 0;
}

int write_cdu(HBA_PORT *port, uint8_t *cdu) {
	uint8_t smart_write_log[8] = {0xd6, 0x01, 0xe0, 0x4f, 0xc2, 0x00, 0xb0, 0x00};
	uint8_t smart_write_log2[8] = {0xd6, 0x06, 0xe1, 0x4f, 0xc2, 0x00, 0xb0, 0x00};
	uint8_t write_buf[512] = {0x08, 0xc0, 0x01, 0x00};
	if (ata_command(port, smart_write_log, write_buf, 512, WRITE_TO_DEV))
		if (ata_command(port, smart_write_log2, cdu, 3072, WRITE_TO_DEV))
			return 1;
	return 0;
}

int clear_smart(HBA_PORT *port) {
	uint8_t smart_write_log[8] = {0xd6, 0x01, 0xe0, 0x4f, 0xc2, 0x00, 0xb0, 0x00};
	uint8_t write_buf[512] = {0x00, 0xc1};
	if (ata_command(port, smart_write_log, write_buf, 512, WRITE_TO_DEV))
		return 1;
	return 0;
}

int secure_erase(HBA_PORT *port) {
	IDENTIFY_DEVICE_DATA id;
	identify_device(port, &id);
	
	// check if security supported
	if (id.sec_supported) {
		uint8_t pass[32] = "SPCLR";
		
		// check if frozen
		if (id.sec_frozen) {
			broadcast_msg("Security is FROZEN. Power cycle the drive and try again...", 1, 1, 1+8, 4+8);
			return 0;
		}
		// check if locked
		if (id.sec_locked) {
			// do login bullshit
			broadcast_msg("Security is LOCKED. TODO: allow users to unlock...", 1, 1, 1+8, 4+8);
			return 0;
		}
		
		// check if enabled
		if (id.sec_enabled==0) {
			if (security_set_password(port, 0, 0, pass) == 0) {
				broadcast_msg("Error when setting password. Aborting...", 1, 1, 1+8, 4+8);
				return 0;
			}
		}
		
		// prepare erase
		char yes_no[2][100] = {"Yes", "No"};
		int result = selection_prompt("Continue with security erase?", yes_no, 2, 1);
		if (result == 0) {
			if (security_erase_prepare(port)) {
				if (security_erase_unit(port, 0, 0, pass)) {
					uint16_t millis = 0;
					uint8_t seconds = 0;
					uint32_t minutes = 0;
					char time_string[100];
					
					while (1) {
						if (port->ci == 0) break;
						msleep(1);
						millis++;
						
						if (millis>=1000) {
							millis = 0;
							seconds++;
							
							sstring(time_string, "%d:%d elapsed. Expected %d minutes...", minutes, seconds, id.sec_erase_time*2);
							broadcast_msg(time_string, 1, 1, 1+8, 4+8);
						}
						if (seconds>=60) {
							seconds = 0;
							minutes++;
						}
					}
					
					broadcast_msg("Security erase sucessful!", 1, 1, 1+8, 0+8);
					return 1;
				}
				else {
					broadcast_msg("Security erase unsuccessful...", 1, 1, 1+8, 4+8);
					return 0;
				}
			}
			else {
				broadcast_msg("Error when preparing erase. Aborting...", 1, 1, 1+8, 4+8);
				return 0;
			} 
		}
		else {
			return 0;
		}

	}
	broadcast_msg("Security is not supported on this drive. Aborting...", 1, 1, 1+8, 4+8);
	return 0;
}