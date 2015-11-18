#include <pit.h>
#include <pci.h>
#include <io.h>
#include <stdint.h>
#include <ahci.h>


uint16_t pci_read_word(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset) {
	uint64_t address;
    uint64_t lbus = (uint64_t)bus;
    uint64_t lslot = (uint64_t)slot;
    uint64_t lfunc = (uint64_t)func;
    uint16_t tmp = 0;
    address = (uint64_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
    outl (0xCF8, address);
    tmp = (uint16_t)((inl (0xCFC) >> ((offset & 2) * 8)) & 0xffff);
    return (tmp);
}

uint16_t getVendorID(uint16_t bus, uint16_t device, uint16_t function) {
        uint32_t r0 = pci_read_word(bus,device,function,0);
        return r0;
}

uint16_t getDeviceID(uint16_t bus, uint16_t device, uint16_t function) {
        uint32_t r0 = pci_read_word(bus,device,function,2);
        return r0;
}

uint16_t getClassId(uint16_t bus, uint16_t device, uint16_t function) {
        uint32_t r0 = pci_read_word(bus,device,function,0xA);
        return (r0 & ~0x00FF) >> 8;
}

uint16_t getSubClassId(uint16_t bus, uint16_t device, uint16_t function) {
        uint32_t r0 = pci_read_word(bus,device,function,0xA);
        return (r0 & ~0xFF00);
}

/*
void pci_probe() {
	for(uint32_t bus = 0; bus < 256; bus++) {
        for(uint32_t slot = 0; slot < 32; slot++) {
            for(uint32_t function = 0; function < 8; function++) {
				uint16_t vendor = getVendorID(bus, slot, function);
				if(vendor == 0xffff) continue;
				uint16_t device = getDeviceID(bus, slot, function);
				
				uint8_t classid = getClassId(bus, slot, function);
				uint8_t subclassid = getSubClassId(bus, slot, function);
				if (classid==1 && subclassid==6) {
					tprinth("device: 0x", device, 4);
					tprinth(" class: 0x", classid, 2);
					tprinth(" subclass: 0x", subclassid, 2);
					tprint("\n");
					
					tprint("SERIAL ATA DEVICE FOUND (AHCI)\n");
					uint32_t ABAR =  pci_read_word(bus,slot,function,0x24) | (pci_read_word(bus,slot,function,0x26) << 16);
					
					tprinth("ABAR: 0x", ABAR, 8);
					tprint("\n");
					
					probe_ahci(ABAR);
				}
            }
        }
    }
}
*/

void pci_probe_for_ahci(AHCI_DEV_UI *ahci_dev) {
	for(uint32_t bus = 0; bus < 256; bus++) {
        for(uint32_t slot = 0; slot < 32; slot++) {
            for(uint32_t function = 0; function < 8; function++) {
				uint16_t vendorid = getVendorID(bus, slot, function);
				uint16_t deviceid = getDeviceID(bus, slot, function);
				if(vendorid == 0xffff) continue;
				
				uint8_t classid = getClassId(bus, slot, function);
				uint8_t subclassid = getSubClassId(bus, slot, function);
				if (classid==1 && subclassid==6) {
					ahci_dev->ABAR = pci_read_word(bus,slot,function,0x24) | (pci_read_word(bus,slot,function,0x26) << 16);
					
					ahci_dev->bus = bus;
					ahci_dev->slot = slot;
					ahci_dev->function = function;
					
					ahci_dev->vendorid = vendorid;
					ahci_dev->deviceid = deviceid;
					
					break;
				}
            }
        }
    }
}

void pci_init() {

}
