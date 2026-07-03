#include "hexioctrl.h"
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "(PCI_DEVS)pci_codes.h"

int main() {
	ALLOW_IO_OPERATIONS;
	int reg=0x00;
	for (int bus=0; bus<256;bus++){
		for(int device = 0; device < 32; device++){
		    for(int function = 0; function < 8; function++){
				unsigned long configAddress = (reg << 2) + (function << 8) + (device << 11) + (bus << 16) + (1 << 31);
				_outpd(0x0CF8, configAddress);
				unsigned long ConfigData = _inpd(0x0CFC);   
				if (ConfigData == 0x0FFFFFFFF) 
					break;
				unsigned long DeviceID = ConfigData >> 16;               
				unsigned long VendorID = ConfigData - (DeviceID << 16);
				printf("\nDevice_ID Vendor_ID\n");
				printf("%9x %9x\n", DeviceID, VendorID);
				for(int i=0;i<PCI_DEVTABLE_LEN;i++){
					if(PciDevTable[i].VenId == VendorID && PciDevTable[i].DevId == DeviceID)
						printf("\n%s, %s\n\n", PciDevTable[i].Chip,PciDevTable[i].ChipDesc);
				}
				for(int i=0;i<PCI_VENTABLE_LEN;i++){
					if(PciVenTable[i].VenId == VendorID)
						printf("%s\n",PciVenTable[i].VenFull);
				}
				
				printf("\n_________________________________________\n\n");		
	  }
	}	
  }
	system("pause");
	return 0;
}