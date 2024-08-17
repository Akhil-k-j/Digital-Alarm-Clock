#include"header.h"
#define SI ((I2CONSET>>3)&1)
void i2c_init(void)
{
	PINSEL0|=0x50;
	I2SCLH=I2SCLL=75;
	I2CONSET=1<<6;
}

void i2c_byte_write_frame(u8 sa,u8 mr,u8 data)
{
	/*Start condition*/
	I2CONSET=(1<<5);
	I2CONCLR=(1<<3);
	while(SI==0);
	I2CONCLR=(1<<5);
	
	/*Send sa+w and check acknowledge*/
	I2DAT=sa;
	I2CONCLR=(1<<3);
	while(SI==0);
	if(I2STAT==0x20)
	{
		uart0_tx_string("Error in SA+W operation\r\n");
		goto exit;
	}
	
	/*Send mr and check acknowledge*/
	I2DAT=mr;
	I2CONCLR=(1<<3);
	while(SI==0);
	if(I2STAT==0x30)
	{
		uart0_tx_string("Error in mr operation\r\n");
		goto exit;
	}
	
	/*Sending the actual data*/
	I2DAT=data;
	I2CONCLR=(1<<3);
	while(SI==0);
	if(I2STAT==0x30)
	{
		uart0_tx_string("Error in data operation\r\n");
		goto exit;
	}
	
	/*Generate stop condition*/
	exit:
	I2CONCLR=(1<<3);
	I2CONSET=(1<<4);
}

u8 i2c_byte_read_frame(u8 sa,u8 mr)
{
	u8 temp;
		
	/*Start condition*/
	I2CONSET=(1<<5);
	I2CONCLR=(1<<3);
	while(SI==0);
	I2CONCLR=(1<<5);
	
	/*Send sa+w and check acknowledge*/
	I2DAT=sa;
	I2CONCLR=(1<<3);
	while(SI==0);
	if(I2STAT==0x20)
	{
		uart0_tx_string("Error in SA+W operation\r\n");
		goto exit;
	}
	
	/*Send mr and check acknowledge*/
	I2DAT=mr;
	I2CONCLR=(1<<3);
	while(SI==0);
	if(I2STAT==0x30)
	{
		uart0_tx_string("Error in mr operation\r\n");
		goto exit;
	}
	
	/*Re-start condition*/
	I2CONSET=(1<<5);
	I2CONCLR=(1<<3);
	while(SI==0);
	I2CONCLR=(1<<5);
	
	/*Send sa+r and check for acknowledge*/
	I2DAT=sa|1;
	I2CONCLR=(1<<3);
	while(SI==0);
	if(I2STAT==0x48)
	{
		uart0_tx_string("Error in sa+w operation\r\n");
		goto exit;
	}
	
	/*read data*/	
	I2CONCLR=(1<<3);
	while(SI==0);
	temp=I2DAT;
	
	/*Generate stop condition*/
	exit:
	I2CONCLR=(1<<3);
	I2CONSET=(1<<4);

	return temp;
}

int i2c_slave_detect(u8 sa)
{
	u32 STAT=0;
	I2CONCLR=(1<<5);
	I2CONSET=(1<<5);
	while(SI==0);
	I2CONCLR=(1<<5);

	sa<<=1;
	I2DAT=(sa);
	I2CONCLR=(1<<3);
	while(SI==0);
	STAT=I2STAT;		
	
	I2CONCLR=(1<<3);
	I2CONSET=(1<<4);

	if(STAT==0x18)
	return sa;
	else
	return 0;
}

void i2c_detect(void)
{
	int c=0,sa=0,i;
	for(i=1;i<=127;i++)
	{  	
		sa=i2c_slave_detect(i);
		if(sa)
		{
			uart0_tx_integer(sa);	
			uart0_tx_string("\r\n");
			c++;
			sa=0;
		}
	}
	uart0_tx_string("Total Devices Connected:");
	uart0_tx_integer(c);
}
