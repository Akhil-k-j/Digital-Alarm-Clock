#include"header.h"
extern int flag; 
extern int flag2; 
 
int alarm_flag; 
int AHH,AMM,ASS,AAMPM; 
 

int main()
{ 
        init(); 
        //flag=0; Need to intialize to 0 if using the active high edge trigger mode (MC Bug), 
                        //  However, in this prgrm we will use Active low  edge trigger. 
         
        ///////////////To write the data in 12 hr format//////////////////////////////////////// 
        ////////////////////If already in 12 hr format not required/////////////////////////// 
        ///////////////////////////////// 11:59:57 pm/////////////////////////////////////// 
        ////////////////////////29/february/2024 THUE/////////////////////////////////////////// 
         
        /*i2c_byte_write_frame(0xD0,0x2,0x71);  //setting hours with am/pm  =>11 pm
        i2c_byte_write_frame(0xD0,0x1,0x59);    //setting minutes
        i2c_byte_write_frame(0xD0,0x0,0x57);    //setting seconds 
         
        //No need of providing the hexa value in the below fields 
        i2c_byte_write_frame(0xD0,0x3,5);               //setting day 
        i2c_byte_write_frame(0xD0,0x4,6);           //setting date 
        i2c_byte_write_frame(0xD0,0x5,10);              //setting month 
        i2c_byte_write_frame(0xD0,0x6,22);      //setting year    */ 
         
        ////////////////////////////////////////////////////////// 
 
        IOCLR0=(1<<21); 
        while(1) 
        { 
        if(flag==1) 
        { 
                IOCLR0=(1<<17); 
                setting_mode(); 
                IOSET0=(1<<17); 
                lcd_cmd(0x01); 
        } 
        if(flag2==1) 
        { 
                IOCLR0=(1<<18); 
                lcd_cmd(0x01); 
                set_alarm(); 
                IOSET0=(1<<18); 
                lcd_cmd(0x01); 
        } 
        read_print(); 
         
         
        if(alarm_flag==1) 
        { 
                if( (AHH==i2c_byte_read_frame(0xD0,0x08))    &&   (AMM==i2c_byte_read_frame(0xD0,0x09)) && (ASS==i2c_byte_read_frame(0xD0,0x0A))  && (AAMPM==i2c_byte_read_frame(0xD0,0x0B)) )       //comparison true 
                { 
                        IOSET0=(1<<21); 
                //      delay_s(2); 
                //      IOCLR0=(1<<21); 
                        alarm_flag=0; 
                } 
        } 
         
                 
        //debug 
        uart0_tx_string("DONE\r\n"); 
        }
} 
 
void init(void) 
{ 
        //Initialization 
        IODIR0=(1<<17)|(1<<18)|(1<<21); 
        IOSET0=(1<<17)|(1<<18); 
        config_eint0();  
        ent0_init();
        lcd_init();
        i2c_init();
        uart0_init(9600); 
} 
 
void read_print(void) 
{ 
        char *AMPM[]={"AM","PM"}; 
        char *DAY[]={"ERR","SUN","MON","TUE","WED","THU","FRI","SAT"}; 
    char *MON[]={"ERR","JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"}; 
        char s,m,h,ampm,day,date,month,year;     
                 
        h=i2c_byte_read_frame(0xD0,0x2);
        m=i2c_byte_read_frame(0xD0,0x1);
        s=i2c_byte_read_frame(0xD0,0x0); 
 
        ampm=i2c_byte_read_frame(0xD0,0x2); 
        day=i2c_byte_read_frame(0xD0,0x3); 
        date=i2c_byte_read_frame(0xD0,0x4); 
        month=i2c_byte_read_frame(0xD0,0x5); 
        year=i2c_byte_read_frame(0xD0,0x6); 

        lcd_cmd(0x80);
        lcd_data(((h>>4)&1)+48);
        lcd_data((h&0xF)+48);
        lcd_data(':'); 
 
        AHH=((((h>>4)&1)*10) + (h&0xf));         
 

        lcd_data((m/0x10)+48);
        lcd_data((m%0x10)+48);
        lcd_data(':'); 
 
        AMM=((((m/0x10)&1)*10) + (m%0x10));

        lcd_data((s/0x10)+48);
        lcd_data((s%0x10)+48); 
        lcd_data(' '); 
 
        ASS=((((s/0x10)&1)*10) + (s%0x10)); 
 
        lcd_string(AMPM[((ampm>>5)&1)]); 
 
        AAMPM=((ampm>>5)&1); 
 
        //Next line 
        lcd_cmd(0xC0); 
        lcd_integer(date); 
                lcd_data('/'); 
        lcd_string(MON[month]); 
                lcd_data('/'); 
        lcd_integer(2000+year); 
                lcd_data(' '); 
        lcd_string((DAY[day])); 
 
 
 
        //              uart0_tx_integer(AHH); 
        //              uart0_tx_integer(AMM); 
        //              uart0_tx_integer(ASS); 
        //              delay_s(2); 
 
} 
