#include"header.h"
int flag;
int flag1;
int flag2;
void EINT0_handler(void)__irq
{
		flag=1;       //On every call Toggles, Initialized with 0 in data segment.
		EXTINT=1;          //To clear the EINT0 intr flag.(So next time also the Interrupt will trigger)
		VICVectAddr=0;          //To finish the ISR execution
}

void EINT1_handler(void)__irq
{
		flag1=1;       //On every call Toggles, Initialized with 0 in data segment.
		EXTINT=2;          //To clear the EINT1 intr flag.(So next time also the Interrupt will trigger)
		VICVectAddr=0;          //To finish the ISR execution
}

void EINT2_handler(void)__irq
{
		flag2=1;       //On every call Toggles, Initialized with 0 in data segment.
		EXTINT=4;          //To clear the EINT2 intr flag.(So next time also the Interrupt will trigger)
		VICVectAddr=0;          //To finish the ISR execution
}


void config_eint0(void)
{
	VICIntSelect=0;      //No FIQ's
	VICVectCntl0=14|(1<<5);   //EINT0 number is 14
	VICVectAddr0=(u32)EINT0_handler;
	VICVectCntl1=15|(1<<5);   //EINT1 number is 15
	VICVectAddr1=(u32)EINT1_handler;
	VICVectCntl2=16|(1<<5);   //EINT2 number is 16
	VICVectAddr2=(u32)EINT2_handler;
	VICIntEnable|=(1<<14)|(1<<15)|(1<<16);		//EINT number are 14/15/16
}
void ent0_init(void)
{
	EXTPOLAR=0;    // All are ACTIVE LOW 
	EXTMODE=7;  //External Interrupt 1 is EDGE TRIGGERED
	PINSEL1|=0x1; 		 //P0.16
	PINSEL0|=0xA0000000;//Since we are using the switch 14,15(GPIO pin P0.14 and P0.15) EINT1 is allocated 
}
