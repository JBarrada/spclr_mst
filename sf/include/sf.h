#ifndef __SF_H_
#define __SF_H_

#include <stdint.h>
#include <ahci.h>

typedef struct tagDIAG_INFO {
	uint8_t diag_revision[4];
	uint8_t access_level[2];
	uint8_t oem_id[4];
	uint8_t cust_id[4];
	uint8_t config_id[8];
	uint8_t firmware_rev[16];
	uint8_t hardware_rev[16];
	uint8_t diag_challenge[256];
} DIAG_INFO;


int get_diag_info(HBA_PORT *port, DIAG_INFO *di);
int is_diag_unlocked(HBA_PORT *port);
int diag_unlock(HBA_PORT *port, uint8_t *key);

int read_cdu(HBA_PORT *port, uint8_t *cdu);
int write_cdu(HBA_PORT *port, uint8_t *cdu);

int clear_smart(HBA_PORT *port);

int secure_erase(HBA_PORT *port);

#endif