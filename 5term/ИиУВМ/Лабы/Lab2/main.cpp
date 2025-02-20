#include "hexioctrl.h"
#include "pci_codes.h"

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

unsigned long getAddress(int bus, int device, int function, int reg) {
	return (reg << 2) + (function << 8) + (device << 11) + (bus << 16) + (1 << 31);
}

void getNames(unsigned long DeviceId, unsigned long VendorId, unsigned long BaseClass, 
			   unsigned long SubClass, unsigned long ProgIf) {
	for(int i = 0; i < PCI_CLASSCODETABLE_LEN; i++){
		if(PciClassCodeTable[i].BaseClass == BaseClass && PciClassCodeTable[i].SubClass == SubClass 
			&& PciClassCodeTable[i].ProgIf == ProgIf)		
			printf("%s (%s %s)\n", PciClassCodeTable[i].BaseDesc, PciClassCodeTable[i].SubDesc, PciClassCodeTable[i].ProgDesc);		
	}

	for(int i = 0; i < PCI_DEVTABLE_LEN; i++){
		if(PciDevTable[i].VenId == VendorId && PciDevTable[i].DevId == DeviceId)		
			printf("%s, %s\n", PciDevTable[i].Chip, PciDevTable[i].ChipDesc);		
	}

	for(int i = 0; i < PCI_VENTABLE_LEN; i++){
		if(PciVenTable[i].VenId == VendorId)		
			printf("%s\n", PciVenTable[i].VenFull);		
	}
}

void showInfoByContext(int bus, int dev, int func) {

	unsigned long configAddress = getAddress(bus, dev, func, 0x00); 
	_outpd(0x0CF8, configAddress);
	unsigned long RegData = _inpd(0x0CFC);   

	if (RegData == -1) 
		return;
	
	unsigned long DeviceID = RegData >> 16;               
	unsigned long VendorID = RegData - (DeviceID << 16);

	configAddress = getAddress(bus, dev, func, 0x08);
	_outpd(0x0CF8, configAddress);
	RegData = _inpd(0x0CFC);

	unsigned long ClassID = RegData >> 8;                             
	unsigned long RevisionID = RegData - (ClassID << 8);

	unsigned long BaseClassCode = ClassID >> 16;                   
	unsigned long ProgInterface = ClassID - (BaseClassCode << 16); 
	unsigned long SubClassCode = ProgInterface >> 8;               
	ProgInterface = ProgInterface - (SubClassCode << 8);           

	configAddress = getAddress(bus, dev, func, 0x2C);
	_outpd(0x0CF8, configAddress);
	RegData = _inpd(0x0CFC);

	unsigned long SubsysID = RegData >> 16;
	unsigned long SubsysVendID = RegData - (SubsysID << 16);

	getNames(DeviceID, VendorID, BaseClassCode, SubClassCode, ProgInterface);

	printf("\nDevice_ID Vendor_ID\n");
	printf("%9x %9x\n", DeviceID, VendorID);
	printf("\n_________________________________________\n\n");	
}

void info(){
	for(int device = 0; device < 32; ++device){
		for(int function = 0; function < 8; ++function){
			showInfoByContext(0, device, function);	
		}
	}	
}

int main() {
	ALLOW_IO_OPERATIONS;

	info();
	
	system("pause");
	return 0;
}