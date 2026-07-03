#include <dos.h>
#include <conio.h>
#include <stdio.h>

const unsigned char QUIT_CHAR = 0x01;
const unsigned char HIGHLIGHT_CHAR = 0xa3;
const int TARGET_INTERRUPT = 9;
const int true = 1;
const int false = 0;

void interrupt NewInterrupt(void);
void interrupt (*oldInterrupt)(void);
void SaveOldInterrupt();
void SetNewInterrupt();
void RestoreOldInterrupt();
void UpdateHighlightFlag(unsigned char);
void UpdateQuitFlag(unsigned char);
void WaitInputFree();
void SetMask(unsigned char mask);
void Highlight (void); 
void EndInterrupt();

int commandIsExecuted;
int quitFlag;
int needHighlight;

void main()
{	
    commandIsExecuted = false;
	quitFlag = false;
	needHighlight = false;
	
	clrscr();
	
	SaveOldInterrupt();
	SetNewInterrupt();
	
	while(!quitFlag)
	{
		if (needHighlight)
		{
			Highlight();
			needHighlight = false;
		}
	}		
	
	RestoreOldInterrupt();
	clrscr();
	return;
}

void SaveOldInterrupt()
{
	oldInterrupt = getvect(TARGET_INTERRUPT);
}

void SetNewInterrupt()
{
	setvect(TARGET_INTERRUPT, NewInterrupt);	
}

void RestoreOldInterrupt()
{
	setvect(TARGET_INTERRUPT, oldInterrupt);
}

void interrupt NewInterrupt()
{
	unsigned char value = 0;
	oldInterrupt();
		
	value = inp(0x60);
	
	UpdateQuitFlag(value);
		
	UpdateHighlightFlag(value);
	
	commandIsExecuted = commandIsExecuted || (needHighlight == false) || (value == 0xFA);
	
	printf("\t%x", value);
	
	EndInterrupt();
}

void EndInterrupt()
{
	outp(0x20, 0x20);
}

void UpdateQuitFlag(unsigned char value)
{
	if (value == QUIT_CHAR) 
	{
		quitFlag = true;
	}
}

void UpdateHighlightFlag(unsigned char value)
{
	if (value != HIGHLIGHT_CHAR)
	{
		return;
	}
	
	if (needHighlight)
	{
		needHighlight = false;
	}
	else
	{
		needHighlight = true;
	}	
}

void SetMask(unsigned char mask)
{
	commandIsExecuted = false;
	
	while (!commandIsExecuted)
	{		
		WaitInputFree();
		outp(0x60, 0xED);		
		outp(0x60, mask);
		printf("\tset mask %d", mask);
		delay(50);		
	}		
}

void WaitInputFree()
{
	while((inp(0x64) & 0x02) != 0x00);
}

void Highlight ()
{
	printf("\tBegin highligt");
	SetMask(0x02);
	delay(1000);
	SetMask(0x00);
	delay(1000);
	SetMask(0x02);
	delay(500);
	SetMask(0x00);
	delay(1000);
	printf("\tEnd highlight");	
}