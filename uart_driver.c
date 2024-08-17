#include"header.h"


void uart0_init(u32 baud)
{
	u32 a[]={15,60,30,15,15};  
	u32 pclk=a[VPBDIV]*1000000;
	u32 result=0;
	result=pclk/(16*baud);  //Adaptive to pclk
	PINSEL0|=0x5;     // Since uart0 is in port 0;
	U0LCR=0x83;	  //  Unlocking DLAB  to change the Baud rate
	U0DLL=result&0xFF;		 // LOWER (1) Byte loaded to DLL (1 byte register) (first lower)
	U0DLM=(result>>8)&0xFF;  // HIGHER (2) Byte loaded to DLM (1 byte register) (masking for safer side);
	U0LCR=0x03;    // Locking DLAB                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
}
#define THRE  ((U0LSR>>5)&1)
void uart0_tx(u8 data)
{
 	U0THR=data;
	while(THRE==0);
}
#define RDR (U0LSR&1)
u8 uart0_rx(void)
{
	while(RDR==0);
	return U0RBR;
}
void uart0_tx_string(s8 *ptr)
{
		while(*ptr)
		{  
		U0THR=*ptr;   	   //single line logic??
		while(THRE==0);
		ptr++;	   		  	        	
		}
}
void uart0_rx_string(u8 *ptr,u8 len)//len is 1 less value of the actual size of the array.
{
	u8 i;
	for(i=0;i<len;i++)// Loop limited with the given size of array
	{
	ptr[i]=uart0_rx();//(blocks the program)collects the data char by char in every iteration
//	while(RDR==0);	 single line code is working so commenting.
//	ptr[i]=U0RBR;		 
	if(ptr[i]=='\r')//if enter encountered it breaks. take input from serial terminal with \r only
	break;
//	else			    //additional not mandatory 
//	uart0_tx(ptr[i]);	 // additional not mandatory
	}
	ptr[i]='\0'; //explicitely printing '\0'.
}


void uart0_tx_integer(int num)
{
	int a[16],i=0;//16 for safer side, maximum Integer data type can hold upto 10 digit
	if(num==0)
	{
		uart0_tx('0');
		return;
	}
	if(num<0)
	{
		uart0_tx('-');
		num=-num;  //don't forget
	}
	while(num>0)       
	{
		a[i]=num%10+48;
		i++;num/=10;
	}
	for(i--;i>=0;i--)    // printing in the reverse order to get the expected output.
	uart0_tx(a[i]);        
}




void uart0_tx_float(float x)
{
	int y;
		int i;
  char a[16]=""; //empty array
	char d[14]=""; //empty array for decimal
	y=(int)x;
	if(x==0)                    // T.case:0
	{
		uart0_tx_string("0.000000");   //directly display 0 	
	return ;
	}
	if(x<0)                     // T.case:-100.001      
	{
		uart0_tx('-');     //printing '-'
		y=-y;
		x=-x;         //converting the sign to positive   -100.001  to 100.001
	}	 
	if(x<1)         // T.case:0.001
	{
		uart0_tx_string("0."); //directly printing 0.  
		y=(x+1)*1000000;    //(0.001+1.000)*1000000=1,001000    (*1000000 for pricission) 
		for(i=0;y;y/=10,i++)
			a[i]=(y%10)+48;      
		for(i=i-2;i>=0;i--)
			uart0_tx(a[i]);  //reverse printing
		return ;
	}			
	uart0_tx_integer(y);
	y=x;
	y=(((x-y)+1)*1000000);         //converting into 1. decimal 
	uart0_tx('.');              //explicitly printing the '.'
	for(i=0;y;y/=10,i++)
		d[i]=y%10+48;
	for(i=i-2;i>=0;i--)   // i=i-2 -> neglect 1 
		uart0_tx(d[i]);    //reverse printing decimal value
}






