#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

void WriteStringToScreen(char* str, int x, int y, unsigned char attribute);
void ByteToBinaryString(unsigned char temp, char *str);
void ShowICsStatuses();
void interrupt new_8(void);
void interrupt new_9(void);
void interrupt new_10(void);
void interrupt new_11(void);
void interrupt new_12(void);
void interrupt new_13(void);
void interrupt new_14(void);
void interrupt new_15(void);
void interrupt new_70(void);
void interrupt new_71(void);
void interrupt new_72(void);
void interrupt new_73(void);
void interrupt new_74(void);
void interrupt new_75(void);
void interrupt new_76(void);
void interrupt new_77(void);
void interrupt (*old_8)(void);
void interrupt (*old_9)(void);
void interrupt (*old_10)(void);
void interrupt (*old_11)(void);
void interrupt (*old_12)(void);
void interrupt (*old_13)(void);
void interrupt (*old_14)(void);
void interrupt (*old_15)(void);
void interrupt (*old_70)(void);
void interrupt (*old_71)(void);
void interrupt (*old_72)(void);
void interrupt (*old_73)(void);
void interrupt (*old_74)(void);
void interrupt (*old_75)(void);
void interrupt (*old_76)(void);
void interrupt (*old_77)(void);
void RestoreInterrupts();

	
void main()
{		
    unsigned char value;
	// изменяем таблицу векторов аппаратных прерывания
	old_8 = getvect(0x8);      // IRQ0 прерывание таймера, возникает 18,2 раза в секунду.
	old_9 = getvect(0x9);      // IRQ1 прерывание от клавиатуры.
	old_10 = getvect(0xA);     // IRQ2 используется для каскадирования аппаратных прерываний
	old_11 = getvect(0xB);     // IRQ3 прерывание асинхронного порта COM2.
	old_12 = getvect(0xC);     // IRQ4 прерывание асинхронного порта COM1.
	old_13 = getvect(0xD);     // IRQ5 прерывание от контроллера жесткого диска для XT.
	old_14 = getvect(0xE);     // IRQ6 прерывание генерируется контроллером флоппи диска после завершения операции
	old_15 = getvect(0xF);     //IRQ7 прерывание принтера.

	old_70 = getvect(0x70);    //IRQ8 прерывание от часов реального времени.
	old_71 = getvect(0x71);    //IRQ9 прерывание от контроллера EGA.
	old_72 = getvect(0x72);    //IRQ10 зарезервировано.
	old_73 = getvect(0x73);    //IRQ11 зарезервировано.
	old_74 = getvect(0x74);    //IRQ12 зарезервировано.
	old_75 = getvect(0x75);    //IRQ13 прерывание от математического сопроцессора.
	old_76 = getvect(0x76);    //IRQ14 прерывание от контроллера жесткого диска.
	old_77 = getvect(0x77);    //IRQ15 зарезервировано.
		
	setvect(0x78, new_8);
	setvect(0x79, new_9);
	setvect(0x7A, new_10);
	setvect(0x7B, new_11);
	setvect(0x7C, new_12);
	setvect(0x7D, new_13);
	setvect(0x7E, new_14);
	setvect(0x7F, new_15);

	setvect(0x80, new_70);
	setvect(0x81, new_71);
	setvect(0x82, new_72);
	setvect(0x83, new_73);
	setvect(0x84, new_74);
	setvect(0x85, new_75);
	setvect(0x86, new_76);
	setvect(0x87, new_77);
	
	//                                        ИНИЦИАЛИЗАЦИЯ
	//     Ведущий контроллер
	value = inp(0x21); // Запоминаем регистр масок
	outp(0x20, 0x11);   // ICW1 (инициализация ведущего контроллера) - ICW4 будет вызвана, 
					    // размер вектора прерывания - 8 байт
	outp(0x21, 0x78);   // ICW2
	outp(0x21, 0x04);   // ICW3
	outp(0x21, 0x01);   // ICW4
	outp(0x21, value);  // Восстанавливаем регистр масок
		
	//     Ведомый контроллер
	value = inp(0xA1);  // Запоминаем регистр масок
	outp(0xA0, 0x11);   // ICW1
	outp(0xA1, 0x80);   // ICW2
	outp(0xA1, 0x02);	// ICW3
	outp(0xA1, 0x01);	// ICW4
	outp(0xa1, value);  // Восстанавливаем регистр масок
	clrscr();	
	_dos_keep(0,(_DS-_CS)+(_SP/16)+1); // Оставить программу резидентной
}
	

void ByteToBinaryString(unsigned char temp, char *str)
{
	int i;
	str[8] = 0;
	i=7;
	
	while(temp)
	{
		str[i]='0'+temp%2;
		temp=temp/2;
		i--;
	}
	
	for(;i>-1;i--)
		str[i]='0';
}	
	
void WriteStringToScreen(char* str, int x, int y, unsigned char attribute)
{
	int i = 0;
	char far* start = (char far*)0xb8000000;
	start += x+160*y;
	
	while(str[i] != 0)
	{
		*start = str[i];
		start++;
		*start = attribute;
		start++;
		i++;
	}				
}
	
void ShowICsStatuses()
{
	char str[9];
	
	unsigned char isr_master, isr_slave; // Interrupt Service Register - Регистр обслуживаемых прерываний
	unsigned char irr_master, irr_slave; // Interrupt Request Register - Регистр запросов на прерывания
	unsigned char imr_master, imr_slave; // Interrupt Mask Register  - Регистр масок
	
	imr_master = inp(0x21);
	imr_slave = inp(0xA1);

	outp(0x20, 0x0A);
	irr_master = inp(0x20);
	outp(0x20, 0x0B);
	isr_master = inp(0x20);

	outp(0xA0,0x0A);
	irr_slave = inp(0xA0);
	outp(0xA0,0x0B);
	isr_slave = inp(0xA0);
						
	WriteStringToScreen("Master IC  ISR: ",0, 0, 0x4E);
	ByteToBinaryString(isr_master, str);
	WriteStringToScreen(str, 30, 0, 0x4E);
		
	WriteStringToScreen("|IRR: ",46, 0, 0x4E);
	ByteToBinaryString(irr_master, str);
	WriteStringToScreen(str, 58, 0, 0x4E);	

	WriteStringToScreen("|MASK: ", 74, 0, 0x4E);
	ByteToBinaryString(imr_master, str);
	WriteStringToScreen(str, 88, 0, 0x4E);	
		
	WriteStringToScreen("Slave IC   ISR: ", 0, 1, 0x4E);
	ByteToBinaryString(isr_slave, str);
	WriteStringToScreen(str, 30, 1, 0x4E);
		
	WriteStringToScreen("|IRR: ", 46, 1, 0x4E);
	ByteToBinaryString(irr_slave, str);
	WriteStringToScreen(str, 58, 1, 0x4E);	

	WriteStringToScreen("|MASK: ",74, 1, 0x4E);
	ByteToBinaryString(imr_slave, str);
	WriteStringToScreen(str, 88, 1, 0x4E);	
}

	
void interrupt new_8(void)
{
	ShowICsStatuses();
	(*old_8)();
}

void interrupt new_9(void)
{
	unsigned char value;
	
	ShowICsStatuses();
	(*old_9)();
	
	value = inp(0x60);
	
	if (value == 0x01)
	{
		RestoreInterrupts();
		clrscr();
	}	
}

void interrupt new_10(void)
{
	ShowICsStatuses();
	(*old_10)();
}
void interrupt new_11(void)
{
	ShowICsStatuses();
	(*old_11)();
}
void interrupt new_12(void)
{
	ShowICsStatuses();
	(*old_12)();
}
void interrupt new_13(void)
{
	ShowICsStatuses();
	(*old_13)();
}
void interrupt new_14(void)
{
	ShowICsStatuses();
	(*old_14)();
}
void interrupt new_15(void)
{
	ShowICsStatuses();
	(*old_15)();
}
void interrupt new_70(void)
{
	ShowICsStatuses();
	(*old_70)();
}
void interrupt new_71(void)
{
	ShowICsStatuses();
	(*old_71)();
}
void interrupt new_72(void)
{
	ShowICsStatuses();
	(*old_72)();
}
void interrupt new_73(void)
{
	ShowICsStatuses();
	(*old_73)();
}
void interrupt new_74(void)
{
	ShowICsStatuses();
	(*old_74)();
}
void interrupt new_75(void)
{
	ShowICsStatuses();
	(*old_75)();
}
void interrupt new_76(void)
{
	ShowICsStatuses();
	(*old_76)();
}
void interrupt new_77(void)
{
	ShowICsStatuses();
	(*old_77)();
}

void RestoreInterrupts()
{
	disable();
 
    setvect(0x78, old_8);
	setvect(0x79, old_9);
	setvect(0x7A, old_10);
	setvect(0x7B, old_11);
	setvect(0x7C, old_12);
	setvect(0x7D, old_13);
	setvect(0x7E, old_14);
	setvect(0x7F, old_15);

	setvect(0x80, old_70);
	setvect(0x81, old_71);
	setvect(0x82, old_72);
	setvect(0x83, old_73);
	setvect(0x84, old_74);
	setvect(0x85, old_75);
	setvect(0x86, old_76);
	setvect(0x87, old_77);

	enable();
}