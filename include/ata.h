#ifndef __ATA1_H_
#define __ATA1_H_

#include <stdint.h>
#include <ahci.h>

typedef struct tagIDENTIFY_DEVICE_DATA {
	uint8_t		general_config[2];
	uint8_t		serial[21];
	uint8_t		firmware[9];
	uint8_t		model[41];
	uint8_t		world_wide[9];
	
	uint8_t		sec_level:1;
	uint8_t		sec_enh_erase_sup:1;
	uint8_t		sec_count_exp:1;
	uint8_t		sec_frozen:1;
	uint8_t		sec_locked:1;
	uint8_t		sec_enabled:1;
	uint8_t		sec_supported:1;
	
	uint16_t	sec_erase_time;
	uint16_t	sec_enh_erase_time;
	
	uint8_t		cmd_feat_supported_83[2];
	
	uint64_t	max_lba;
} IDENTIFY_DEVICE_DATA;

typedef struct tagSMART_DATA {
	uint8_t self_test_status;
	uint8_t smart_capability[2];
	uint16_t self_test_short_poll;
	uint16_t self_test_ext_poll;
} SMART_DATA;

int identify_device(HBA_PORT *port, IDENTIFY_DEVICE_DATA *id);

int smart_read_data(HBA_PORT *port, SMART_DATA *sd);
int smart_execute_offline_immediate(HBA_PORT *port, uint8_t subcommand);
int smart_read_log(HBA_PORT *port, uint8_t log_page, uint8_t *buf, unsigned int count);
int smart_write_log(HBA_PORT *port, uint8_t log_page, uint8_t *buf, unsigned int count);
int smart_enable_operations(HBA_PORT *port);
int smart_disable_operations(HBA_PORT *port);
int smart_return_status(HBA_PORT *port, uint8_t log_page);

int security_set_password(HBA_PORT *port, uint8_t set_master, uint8_t sec_level, uint8_t *password);
int security_unlock(HBA_PORT *port, uint8_t compare_master, uint8_t *password);
int security_erase_prepare(HBA_PORT *port);
int security_erase_unit(HBA_PORT *port, uint8_t compare_master, uint8_t enhanced_erase, uint8_t *password);
int security_freeze_lock(HBA_PORT *port);
int security_disable_password(HBA_PORT *port, uint8_t compare_master, uint8_t *password);

#endif