#include <conio.h>
#include <stdio.h>
#include "hexioctrl.h"

#define _IDENTIFY_DEVICE 0xEC

struct Registers{
	unsigned int DH[2];
	unsigned int AS[2];
	unsigned int CR[2];
	unsigned int DR[2];
}registers = { { 0x1F6, 0x176 }, { 0x3F6, 0x376 }, { 0x1F7, 0x177 }, { 0x1F0, 0x170 } };


bool is_device_available(int channel, int offset){
	return (_inp(registers.AS[channel]) & (1 << offset)) ? false : true;
}


void print_word(unsigned short word){
	printf("%c%c", word >> 8, word & 0xFF);
}


void output_info(int channel, int device){
	unsigned short config_block[256];

	while(!is_device_available(device, 7));
	
	_outp(registers.DH[channel], (device << 4) + (7 << 5)); 
	
	for(int pause = 0; pause < 10000; ++pause){
		if(!is_device_available(channel, 6)) break;
		if(pause == 9999){
			printf("Device %d on %d channel is not availble\n\n\n", device, channel);
			return;
		}
	}

	// request for identifying device
	_outp(registers.CR[channel], _IDENTIFY_DEVICE);

	while(!is_device_available(device, 7));

	if(!(_inp(registers.CR[channel]) & (1  << 3))){
		printf("Device %d on %d channel is not availble\n\n\n", device, channel);
		return;
	}
	
	for(int i = 0; i < 256; ++i){
		config_block[i] = _inpw(registers.DR[channel]);
	}

	
	printf("Model: ");
	for(int i = 27; i < 47; ++i){
		print_word(config_block[i]);
	}
	
	printf("\nProducer: ");
	print_word(config_block[10]);
	

	printf("\nSerial number: ");
	for(int i = 10; i < 20; ++i){
		print_word(config_block[i]);
	}

	printf("\nFirmware: ");
	for(int i = 23; i < 27; ++i){
		print_word(config_block[i]);
	}

	printf("\nSize: ");
	if(!(config_block[0] & (1 << 15))){
		printf("%0.lf", (long double)(((unsigned long*)config_block)[30]) * 512);
	}
	printf(" bytes");

	printf("\nInterface: ");
	if(config_block[0] & (1 << 15)) printf("ATAPI");
	else printf("ATA");

	printf("\nDMA:\n");
	if(config_block[63 & 1]) printf("* mode 0\n");
	if(config_block[63 & 2]) printf("* mode 1\n");
	if(config_block[63 & 4]) printf("* mode 2\n");
	if(config_block[63 & 8]) printf("* mode 3\n");
	if(config_block[63 & 16]) printf("* mode 4\n");
	if(config_block[63 & 32]) printf("* mode 5\n");
	if(config_block[63 & 64]) printf("* mode 6\n");
	if(config_block[63 & 128]) printf("* mode 7\n");


	printf("PIO:\n");
	if(config_block[64 & 1]) printf("* mode 3\n");
	if(config_block[64 & 2]) printf("* mode 4\n");

	printf("\n\n\n");
}



int main(){
	ALLOW_IO_OPERATIONS;
	output_info(0, 0);
	output_info(0, 1);
	output_info(1, 0);
	output_info(1, 1);
	return _getch();
}


