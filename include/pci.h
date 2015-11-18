#ifndef __PCI_H_
#define __PCI_H_

#include <stdint.h>
#include <ahci.h>

void pci_probe();
void pci_init();
void pci_probe_for_ahci(AHCI_DEV_UI *ahci_dev);

#endif