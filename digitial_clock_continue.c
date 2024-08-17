#include"header.h"

extern int flag;
extern int flag1;
extern int flag2;

void setting_mode(void)
{
	char *AMPM[]={"AM","PM"};
	char *DAY[]={"ERR","SUN","MON","TUE","WED","THU","FRI","SAT"};
	char *MON[]={"ERR","JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};
	int HN,LN,HH=1,MM,SS,am_pm,data,h;
	int date,month,year,day;

	int LY=0,BM=0,SPFEB=0,FEB=0;

	flag=flag1=flag2=0;
	lcd_cmd(0x01);
	lcd_string("Setting Mode");
	delay_s(1);
	lcd_cmd(0x01);
	L1:
	h=i2c_byte_read_frame(0xD0,0x2);
	HH=(h&0x1F);
	HH=((HH/16)*10)+(HH%16);
	lcd_cmd(0x01);
	while(1)
	{
			if(flag==1)
			{
				HN=HH/10;
				LN=HH%10;
				HH=((HN<<4)|LN);
				HH=((h&0x60)|HH);
				i2c_byte_write_frame(0xD0,0x2,HH);	//setting hours with am/pm
				flag=0;
				return ;
			}
			if(flag1==1)
			{
				flag1=0;
			   	if(press_count()==1)
				{
				HH++;
				if(HH==13)
				HH=1;
				}
				else
				{
				HH--;
				if(HH==0)
				HH=12;
				}
				flag1=0;
			}
			if(flag2==1)
			{
				if(press_count()==1)
				{
				HN=HH/10;
				LN=HH%10;
				HH=((HN<<4)|LN);
				HH=((h&0x60)|HH);		
				i2c_byte_write_frame(0xD0,0x2,HH);	//setting hours with am/pm
				flag2=0;
				break;
				}
				else
				{
				HN=HH/10;
				LN=HH%10;
				HH=((HN<<4)|LN);
				HH=((h&0x60)|HH);		
				i2c_byte_write_frame(0xD0,0x2,HH);	//setting hours with am/pm
				flag2=0;
				goto L8;		
				}
			}
			lcd_cmd(0x80);
			lcd_string("Enter Hour:");
			lcd_cmd(0xC0);	 
			lcd_integer(HH/10);
			lcd_integer(HH%10);
	}
	L2:	
	MM=i2c_byte_read_frame(0xD0,0x1);
	MM=((MM/16)*10)+(MM%16);
	lcd_cmd(0x01);
	while(1)
	{
			if(flag==1)
			{
				HN=MM/10;
				LN=MM%10;
				MM=((HN<<4)|LN);
				i2c_byte_write_frame(0xD0,0x1,MM);	//minutes
				flag=0;
				return ;
			}
			if(flag1==1)
			{	
				if(press_count()==1)
				{
				MM++;
				if(MM==60)
				MM=0;
				}
				else
				{
				MM--;
				if(MM==-1)
				MM=59;
				}
				flag1=0;
			}
			if(flag2==1)
			{
				if(press_count()==1)
				{
				HN=MM/10;
				LN=MM%10;
				MM=((HN<<4)|LN);
					i2c_byte_write_frame(0xD0,0x1,MM);	//minutes
					flag2=0;
					break;
				}
				else
				{
				HN=MM/10;
				LN=MM%10;
				MM=((HN<<4)|LN);
				i2c_byte_write_frame(0xD0,0x1,MM);	//minutes
				flag2=0;
				goto L1;
				}
			}
			lcd_cmd(0x80);
			lcd_string("Enter minutes:");
			lcd_cmd(0xC0);				 
			lcd_integer(MM/10);
			lcd_integer(MM%10);
	}
	L3:	
	SS=i2c_byte_read_frame(0xD0,0x0);
	SS=((SS/16)*10)+(SS%16);
	lcd_cmd(0x01);
	while(1)
	{
			if(flag==1)
			{
				HN=SS/10;
				LN=SS%10;
				SS=((HN<<4)|LN);	
				i2c_byte_write_frame(0xD0,0x0,SS);	//setting minutes
				flag=0;
				return ;
			}
			if(flag1==1)
			{	
				if(press_count()==1)
				{
				SS++;
				if(SS==60)
				SS=0;
				}
				else
				{
				SS--;
				if(SS==-1)
				SS=59;
				}
				flag1=0;
			}
			if(flag2==1)
			{
				if(press_count()==1)
				{
				HN=SS/10;
				LN=SS%10;
				SS=((HN<<4)|LN);	
				i2c_byte_write_frame(0xD0,0x0,SS);	//setting minutes
				flag2=0;
				break;
				}
					else
					{
					HN=SS/10;
					LN=SS%10;
					SS=((HN<<4)|LN);	
					i2c_byte_write_frame(0xD0,0x0,SS);	//setting minutes
					flag2=0;
					goto L2;
					}
			}
			lcd_cmd(0x80);
			lcd_string("Enter seconds:");
			lcd_cmd(0xC0);
			lcd_integer(SS/10);
			lcd_integer(SS%10);
	}
	L4:
	am_pm=i2c_byte_read_frame(0xD0,0x2);
	data=(am_pm>>5)&1;
	lcd_cmd(0x01);
	while(1)
	{
			if(flag==1)
			{
					if(data)
					am_pm=(am_pm|(1<<5));
					else
					am_pm=((~(1<<5))& am_pm);
				i2c_byte_write_frame(0xD0,0x2,am_pm);	//setting minutes
				flag=0;
				return ;
			}
			if(flag1==1)
			{
				if(data)
				data=0;
				else
				data=1;
				flag1=0;
			}
			if(flag2==1)
			{	
				if(press_count()==1)
				{
					if(data)
					am_pm=(am_pm|(1<<5));
					else
					am_pm=((~(1<<5))& am_pm);
					i2c_byte_write_frame(0xD0,0x2,am_pm);	//setting minutes
					flag2=0;
					break;
				}
				else
				{
					if(data)
					am_pm=(am_pm|(1<<5));
					else
					am_pm=((~(1<<5))& am_pm);
					i2c_byte_write_frame(0xD0,0x2,am_pm);	//setting minutes
					flag2=0;
					goto L3;
				}
			}
			lcd_cmd(0x80);
			lcd_string("AM/PM:");
			lcd_cmd(0xC0);
			lcd_string(AMPM[data]);
	}
	L5:
	year=i2c_byte_read_frame(0xD0,0x6);
	year=2000+year;
	lcd_cmd(0x01);
	while(1)
	{
			if(flag==1)
			{
				year=year%2000;	
				i2c_byte_write_frame(0xD0,0x6,year);
				flag=0;
				return ;
			}
			if(flag1==1)
			{
				if(press_count()==1)
				{	
				year++;
				if(year==2101)
				year=2000;
				}
				else
				{
				year--;
				if(year==1999)
				year=2100;
				}
				flag1=0;
			}
			if(flag2==1)
			{
					if(press_count()==1)
					{	
					year=year%2000;	
					i2c_byte_write_frame(0xD0,0x6,year);
					break;
					}
					else
					{
					year=year%2000;	
					i2c_byte_write_frame(0xD0,0x6,year);
					}
					goto L4;
			}
			lcd_cmd(0x80);
			lcd_string("Enter Year:");
			lcd_cmd(0xC0);
			lcd_integer(year/1000);
			lcd_integer((year/100)%10);
			lcd_integer((year%100)/10);
			lcd_integer(year%10);
	}
	L6:
	month=i2c_byte_read_frame(0xD0,0x5);
	lcd_cmd(0x01);	
	while(1)
	{
			if(flag==1)
			{	
				i2c_byte_write_frame(0xD0,0x5,month);
				flag=0;
				return ;
			}
			if(flag1==1)
			{
				if(press_count()==1)
				{	
				month++;
				if(month==13)
				month=1;
				}
				else
				{
				month--;
				if(month==0)
				month=12;
				}
				flag1=0;
			}
			if(flag2==1)
			{
					if(press_count()==1)
					{
					i2c_byte_write_frame(0xD0,0x5,month);
					flag2=0;
					break;
					}
					else
					{
					i2c_byte_write_frame(0xD0,0x5,month);
					flag2=0;
					goto L5;
					}
			}
			lcd_cmd(0x80);
			lcd_string("Enter Month:");
			lcd_cmd(0xC0);
			lcd_string(MON[month]);
	}
	L7:	
	year=i2c_byte_read_frame(0xD0,0x6);
	month=i2c_byte_read_frame(0xD0,0x5);
	date=i2c_byte_read_frame(0xD0,0x4);
	if(year%4==0)
	LY=1;
	if(month==1 || month==3 || month==5 || month==7 || month==8 || month==10 ||month==12)
	BM=1;
	else
	BM=0;
	if(month==2 && LY==1)
	SPFEB=1;
	else if(month==2 && LY==0)
	FEB=1;
	lcd_cmd(0x01);		 
	while(1)
	{
			if(flag==1)
			{	
				i2c_byte_write_frame(0xD0,0x4,date);
				flag=0;
				return ;
			}
			if(flag1==1)
			{
				if(press_count()==1)
				{
				date++;	
				if(BM==1)
				{
				if(date>=32)
				date=31;
				}
				else if(SPFEB==1)
				{
				if(date>=30)
				date=29;
				}
				else if(FEB==1)
				{
					if(date>=29)
					date=28;
				}
				else if(BM==0)
				{
				if(date>=31)
				date=30;
				}
				}
				else
				{
				date--;
				if(date==0)
				date=1;
				}
				flag1=0;
			}
			if(flag2==1)
			{
				if(press_count()==1)
				{
					i2c_byte_write_frame(0xD0,0x4,date);
					flag2=0;
					LY=0,BM=0,SPFEB=0,FEB=0;
					break;
				}
					else
					{
					i2c_byte_write_frame(0xD0,0x4,date);
					flag2=0;
					LY=0,BM=0,SPFEB=0,FEB=0;
					goto L6;
					}
			}
			lcd_cmd(0x80);
			lcd_string("Enter Date:");
			lcd_cmd(0xC0);
			lcd_integer(date/10);
			lcd_integer(date%10);
	}
	L8:
	day=i2c_byte_read_frame(0xD0,0x3);
	lcd_cmd(0x01);	
	while(1)
	{
			if(flag==1)
			{
				i2c_byte_write_frame(0xD0,0x3,day);
				flag=0;
				return ;
			}
			if(flag1==1)
			{
				if(press_count()==1)
				{	
				day++;
				if(day==8)
				day=1;
				}
				else
				{							
				day--;
				if(day==0)
				day=7;
				}
				flag1=0;
			}
			if(flag2==1)
			{	
				if(press_count()==1)
				{
					i2c_byte_write_frame(0xD0,0x3,day);
					flag2=0;
					break;
				}
				else
					{
					i2c_byte_write_frame(0xD0,0x3,day);
					flag2=0;
					goto L7;
					}
			}
			lcd_cmd(0x80);
			lcd_string("Enter day:");
			lcd_cmd(0xC0);
			lcd_string(DAY[day]);
	}
}
int press_count(void)
{
				int count=1;
				flag1=flag2=0;
				T0PC=0;
				T0PR=(15000000-1);
				T0TC=0;
				T0TCR=1;
				while(T0TC<1)
				{
					if(flag1==1 || flag2==1)
					{
						count++;
						flag1=0;
						flag2=0;
					}
				}
				T0TCR=0;
				if(count==2)
				return 2;
				else
				return 1;
}
