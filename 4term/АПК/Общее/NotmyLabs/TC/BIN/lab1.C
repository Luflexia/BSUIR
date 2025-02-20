#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>

const int TONALITY = 1600;
const int SOUND_DURATION_MILISECONDS = 3000;

void SetTonality (int tonalityHerz);
void ReadStatusWords(void);
void EnableSound();
void DisableSound();
short HerzToShort(int);

void main()
{
	char c;
	clrscr();
	
	outp(0x43, 0xB6); // channel 2, operation 4, mode 3, format 0
					  
	SetTonality(TONALITY);	
	
	printf("Press:\n'1' - Play sound\n'2' - Write status words\n'Esc' - quit\n\n");
	
	while(c != 27)
	{
		c = getch();
		switch(c)
		{
			case '1': 
				EnableSound();	
				delay(SOUND_DURATION_MILISECONDS); 	
				DisableSound();				
				break;
			case '2': 
				ReadStatusWords();
				break;
			case 27: break;
		}
	}				
	
	clrscr();
	return;
}

void EnableSound()
{
	char port61;
	
	port61 = inp(0x61);
	port61 = port61 | 3;
	outp(0x61, port61);	
}
	
void DisableSound()
{
	char port61;
	port61 = port61 & 0xFFFC;
	outp(0x61, port61);
}

void SetTonality (int frequency)
{
	short value = HerzToShort(frequency);

    outp(0x42, (char)value);		// low byte
    outp(0x42, (char)(value >> 8)); // high byte
}

short HerzToShort(int value)
{
	return 1193180/value;
}

void ReadStatusWords()
{
	unsigned char temp;
	char *str;
	int i;
	str=(char*)calloc(9, sizeof(char));
	
	for(i =0;i<3;i++)
	{
		outp(0x43, 0xe2 + (i * (i+1)));				
		temp = inp(0x40+i);				
		itoa(temp, str, 2);
		printf("Channel state word %d: %s\n",i, str);	
	}
	
	free(str);
	
	return;	
}