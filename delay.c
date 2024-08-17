#include"header.h"
void delay_s(unsigned int sec)
{
int a[]={15,60,30,15,15};
unsigned int PCLK=a[VPBDIV]*1000000;
T0PC=0;
T0PR=PCLK-1	 ;
T0TC=0;
T0TCR=1;
while(T0TC<sec);
T0TCR=0;
}

void delay_ms(unsigned int msec)
{
int a[]={15,60,30,15,15};
unsigned int PCLK=a[VPBDIV]*1000;
T0PC=0;
T0PR=PCLK-1	 ;
T0TC=0;
T0TCR=1;
while(T0TC<msec);
T0TCR=0;
}
