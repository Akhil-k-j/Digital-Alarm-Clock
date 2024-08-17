//For rydulabs board//
#include"header.h"
#define LED 0xFE<<16;
#define RS	1<<17;
#define RW	1<<18;                                   
#define EN	1<<19;
void lcd_init(void)
{
	IODIR1=LED;     //remember this is port1 of rydulabs board if its port0 LED is there at 17,18,19 pin
	PINSEL2=0x0;	//for safer side
	IOCLR1=EN;	    //for safety purpose Recommended
	lcd_cmd(0x02);	//choosing 4 bit mode
	lcd_cmd(0x28); 	//double line enable in 4 bit mode
	lcd_cmd(0x0e);	//Cursor enable
	lcd_cmd(0x01);  //clear screen and set cursor at home postion 
}

void lcd_cmd(unsigned char cmd)
{
	IOCLR1=LED;   //only one time 
	IOSET1=((cmd&0xF0)<<16);         //---> Higher nibble
	IOCLR1=RS;	  //0
	IOCLR1=RW;	  //0
	IOSET1=EN;	  //1
	delay_ms(2);
	IOCLR1=EN;	  //off

	IOCLR1=LED;
	IOSET1=((cmd&0x0F)<<20);         //---> Lower nibble
	IOCLR1=RS;		//0
	IOCLR1=RW;		//0
	IOSET1=EN;	    //1
	delay_ms(2);
	IOCLR1=EN;	    //off
	//one entire cmd byte tarnsfered succefully.
}

void lcd_data(unsigned char data)
{
	IOCLR1=LED;   //only one tiume 
	IOSET1=((data&0xF0)<<16);        //-->Higehr nibble
	IOSET1=RS;	 //1
	IOCLR1=RW;	 //0
	IOSET1=EN;   //1
	delay_ms(2);
	IOCLR1=EN;	 //off

	IOCLR1=0xFE<<16; 
	IOSET1=((data&0x0F)<<20);	    //-->Lower nibble
	IOSET1=RS;	 //1
	IOCLR1=RW;	 //0
	IOSET1=EN;   //1
	delay_ms(2);
	IOCLR1=EN;	 //off
	//one entire data byte tarnsferred.
}

void lcd_string(char *ptr)
{
	while(*ptr!=0)
	{
	lcd_data(*ptr);
	ptr++;
	}
}

void lcd_integer(int num)
{
	int a[16],i=0;
	if(num==0)
	{
		lcd_data('0');
		return;
	}
	if(num<0)
	{
		lcd_data('-');
		num=-num;  //don't forget
	}
	while(num>0)       
	{
		a[i]=num%10+48;
		i++;num/=10;
	}
	for(i--;i>=0;i--)    // printing in the reverse order to get the expected output.
	lcd_data(a[i]);        
}


void lcd_float(float x)
{
	int y=x,i;
	char a[16]=""; //empty array
	char d[14]=""; //empty array for decimal
	if(x==0)                    // T.case:0
	{
	//	lcd_string("0.000000");   //directly display 0 
		lcd_string("0.0");  
		return ;
	}
	if(x<0)                     // T.case:-100.001      
	{
		lcd_data('-');     //printing '-'
		y=-y;x=-x;         //converting the sign to positive   -100.001  to 100.001
	}	 
	if(x<1)         // T.case:0.001
	{
		lcd_string("0."); //directly printing 0.  
	//	y=(x+1)*1000000;    //(0.001+1.000)*1000000=1,001000    (*1000000 for pricission)
			y=(x+1)*10;
		for(i=0;y;y/=10,i++)
			a[i]=(y%10)+48;      
		for(i=i-2;i>=0;i--)	 // i=i-2 -> neglect 1 
			lcd_data(a[i]);  //reverse printing
		return ;
	}		
	lcd_integer(y);	
//	for(i=0;y;y/=10,i++) //T.case 0100.0010
//		a[i]=(y%10)+48;
//	for(i--;i>=0;i--)
//		lcd_data(a[i]);	  //reverse printing integer value 
	y=x;
//	y=(((x-y)+1)*1000000);         //converting into 1. decimal
	y=(((x-y)+1)*10);
	lcd_data('.');              //explicitly printing the '.'
	for(i=0;y;y/=10,i++)
		d[i]=y%10+48;
	for(i=i-2;i>=0;i--)   // i=i-2 -> neglect 1 
		lcd_data(d[i]);    //reverse printing decimal value
}

void lcd_hexa(int num)
{
	char x=0xFF,temp;
	int i=0;    /// declare as seperately. (?)
	char string[]="0123456789ABCDEF";     //buffer
	lcd_string("0x");
	for(i=3;i>=0;i--)
	{
		temp=((x<<(i*8))&num)>>(i*8);
		lcd_data(string[(temp&0xf0)>>4]);
		lcd_data(string[temp&0x0f]);
	}
}














