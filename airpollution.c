#include<lpc21xx.h>
#include "adc.h"
#include "serial.h"
#define RS 0x00000100
#define EN 0x00000200

#define F_w_d 10
#define R_e_v 11
#define R_g_t 12
#define L_f_t 13

#define Auto_Mode 14
#define Alert_GSM 15
#define Step_Fall 29 //31//20

#define RELAY1 16
#define RELAY2 25
#define RELAY3 26
#define RELAY4 27

unsigned char comd[6]={0x38,0x0c,0x06,0x01,0x80};
int lcd_loop,lcd_temp,x_val,y_val,a_val;
int lcd_array[4],delay_count;
int sms_sent=0,co=0;

unsigned int ultra_count=0,ultra_count_1=0,ultra_count_2=0,ultra_count_3=0,i,k,u,v;
unsigned long int pressure,data,data1;
char lcd_data(unsigned char *d);
void lcd_command(unsigned char cmd);
void lcd_data_send(unsigned char dat);

int U_S_(int t, int e);
void sensor_s(void);
void sensor_s2(void);
void sms_alert(void);
void sms_alert1(void);
void sms_alert2(void);
void sms_delay(void);
void time(unsigned);

void lcd_delay(unsigned int z)
{
while(--z);
}
int process_adc1(void)
{

unsigned short adc_value = 0;
  adc_value = adc_read(ADC0, CHANNEL_6);
data=adc_value;
    return data;
}
int process_adc2(void)
{

unsigned short adc_value = 0;
  adc_value = adc_read(ADC0, CHANNEL_7);
   data=adc_value;
   return data;

}
int process_adc3(void)
{
unsigned short adc_value = 0;
  adc_value = adc_read(ADC0, CHANNEL_4);
data=adc_value;
  return data;
}
int process_adc4(void)
{

unsigned short adc_value = 0;
  adc_value = adc_read(ADC0, CHANNEL_3);
data=adc_value;  
  return data;
}
int process_adc5(void)
{
//int data;
unsigned short adc_value = 0;
  adc_value = adc_read(ADC0, CHANNEL_1);
data=adc_value;  
   return data;
 
}
void lcd_command(unsigned char cmd)
{
IOCLR0=RS;
IOSET1=cmd<<17;
IOSET0=EN;

lcd_delay(60000);
IOCLR0=EN;
IOCLR1=cmd<<17;//IOCLR1=cmd|IOSET1;
}

char lcd_data(unsigned char *d)
{
while(*d!='\0')
{
lcd_data_send(*d);
d++;
}
IOCLR0=0xffffffff; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
return(0);
}


void lcd_data_send(unsigned char dat)
{
IOSET0=RS;
IOSET1=dat<<17;
//IOSET0=IOCLR0;
IOSET0=EN;
lcd_delay(12000);
IOCLR0=EN;
IOCLR1=dat<<17;
}

void lcd_putval(int lcd_pnum)
{
lcd_temp=lcd_pnum;
for(lcd_loop=0;lcd_loop<=2;lcd_loop++)
{
lcd_array[lcd_loop]=lcd_temp%10;
lcd_temp=lcd_temp/10;
 
}
for(lcd_loop=2;lcd_loop>=0;lcd_loop--)
{
  IOSET0=RS;
IOSET1=(lcd_array[lcd_loop]+0x30)<<17;
//IOSET0=IOCLR0;
IOSET0=EN;
lcd_delay(12000);
IOCLR0=EN;
IOCLR1=(lcd_array[lcd_loop]+0x30)<<17;
//while(!(U0LSR & 0x20));
//U0THR=array[loop]+0x30; //uart0_putnum(value[loop]);
}
}

void time(unsigned int k)
 {
unsigned int u,v;
for(u=0;u<k;u++)
for(v=0;v<3000;v++);
 }

void sensor_s()
{
a_val=process_adc1(); //adc1 = p0.4
/* x_val=process_adc3(); //adc3 = p0.25
y_val=process_adc4(); //adc4 = p0.30   */
    a_val=((a_val*0.00097715)*300);  
lcd_delay(60000);

    y_val=process_adc5(); //adc1 = p0.4
/* x_val=process_adc3(); //adc3 = p0.25
y_val=process_adc4(); //adc4 = p0.30   */
    y_val=((y_val*0.00097715)*300);  
lcd_delay(60000);  

lcd_delay(60000);
lcd_command(0x80);
lcd_data("TEMP_RED:");
lcd_putval(a_val);
lcd_data("C");
//  lcd_command(0xc0);
// lcd_data("Co2_RED:");
// lcd_putval(y_val);
// lcd_data("ppm");

lcd_delay(60000);  lcd_delay(60000);lcd_delay(60000);
  if(a_val>40)                                                                //Temperature Function//
{  
IOCLR1=0x80000000;
IOSET1=0x10000000;
lcd_command(0x01);
lcd_command(0x80);
lcd_data(" TEMPERATURE HIGH ");
lcd_command(0xc0);
lcd_data("MSG Sending......");
sms_alert2();
lcd_delay(60000); lcd_delay(60000);lcd_delay(60000);
lcd_command(0x01);
}
      else
{
IOSET1=0x80000000;
IOCLR1=0x10000000;
}
 lcd_delay(60000); lcd_delay(60000);lcd_delay(60000); lcd_delay(60000); lcd_delay(60000);lcd_delay(60000);


//      if(y_val>25)
// {
// co=co+1;
// time(1000);
// lcd_delay(60000);
// lcd_command(0x01);
// lcd_command(0x80);
// lcd_data("time:");
// lcd_putval(co);
//    lcd_data("S");
// time(500);
// if(co>10)
// {
// IOCLR1=0x80000000;
// lcd_command(0x01);
// lcd_command(0x80);
// lcd_data(" Co2 HIGH ");
// lcd_command(0xc0);
// lcd_data("MSG Sending......");
// sms_alert1();
// lcd_delay(60000); lcd_delay(60000);lcd_delay(60000);
// lcd_command(0x01);
// }
//     else
// {
// IOSET1=0x80000000;
// }
//  lcd_delay(60000); lcd_delay(60000);lcd_delay(60000); lcd_delay(60000); lcd_delay(60000);lcd_delay(60000);  
//      
//}
}

void sms_alert()              //warning msg alert sms function//
{  
  sms_delay(); // sms_delay(); sms_delay();
uart0_puts1("AT\x0D");
uart0_puts1("\x0a");
sms_delay(); // sms_delay(); sms_delay();
uart0_puts1("AT+CMGF=1\x0D");uart0_puts1("\x0a");
sms_delay();// sms_delay(); sms_delay();
uart0_puts1("AT+CMGS=\"8870361341\"\x0D");uart0_puts1("\x0a");
sms_delay();// sms_delay(); sms_delay();
uart0_puts1("Co2 Level Warning Msg \x0D");uart0_puts1("\x0a");
uart0_puts1("12.965350,N,\x0D");uart0_puts1("\x0a");
uart0_puts1("77.535468,E\x0D");uart0_puts1("\x0a");
sms_delay(); // sms_delay(); sms_delay();
uart0_puts1("\x1a");
sms_delay(); // sms_delay(); sms_delay();sms_delay(); sms_delay(); sms_delay(); sms_delay();
sms_delay();// sms_delay(); sms_delay(); sms_delay();
}


void sms_alert1()        //co2 abnormal msg function//
{  
    sms_delay(); // sms_delay(); sms_delay();
uart0_puts1("AT\x0D");
uart0_puts1("\x0a");
sms_delay(); // sms_delay(); sms_delay();
uart0_puts1("AT+CMGF=1\x0D");uart0_puts1("\x0a");
sms_delay();// sms_delay(); sms_delay();
uart0_puts1("AT+CMGS=\"9740694926\"\x0D");uart0_puts1("\x0a");
sms_delay();// sms_delay(); sms_delay();
uart0_puts1(" Co2 LEVEL ABNORMAL \x0D");uart0_puts1("\x0a");
uart0_puts1("12.965350,N,\x0D");uart0_puts1("\x0a");
uart0_puts1("77.535468,E\x0D");uart0_puts1("\x0a");
sms_delay(); // sms_delay(); sms_delay();
uart0_puts1("\x1a");
sms_delay(); // sms_delay(); sms_delay();sms_delay(); sms_delay(); sms_delay(); sms_delay();
sms_delay();// sms_delay(); sms_delay(); sms_delay();
}


void sms_alert2()     //temperature msg function//
{  
    sms_delay(); sms_delay(); sms_delay();
uart0_puts1("AT\x0D");
uart0_puts1("\x0a");
sms_delay(); // sms_delay(); sms_delay();
uart0_puts1("AT+CMGF=1\x0D");uart0_puts1("\x0a");
sms_delay();// sms_delay(); sms_delay();
uart0_puts1("AT+CMGS=\"9740694926\"\x0D");uart0_puts1("\x0a");
sms_delay();// sms_delay(); sms_delay();
uart0_puts1(" TEMPERATURE HIGH \x0D");uart0_puts1("\x0a");
uart0_puts1("12.965350,N,\x0D");uart0_puts1("\x0a");
uart0_puts1("77.535468,E\x0D");uart0_puts1("\x0a");
sms_delay(); // sms_delay(); sms_delay();
uart0_puts1("\x1a");
sms_delay(); // sms_delay(); sms_delay();sms_delay(); sms_delay(); sms_delay(); sms_delay();
sms_delay();// sms_delay(); sms_delay(); sms_delay();
}

//12.965350, 77.535468
void sms_delay()
{
for(delay_count=0;delay_count<=3;delay_count++)
{
lcd_delay(60000);
}
}
int j=0,l=0;
int main()          //main function//
{
int z;  
PINSEL0=0x00000000;
PINSEL1=0x00000000;
IODIR0=0x00F0FFE0;
IODIR1=0xFFFE0000;


init_adc0();
uart0_init();
//"Automated Pump System"
for(z=0;z<=4;z++)
{
lcd_command((comd[z]));
  }
lcd_delay(60000);
lcd_command(0x80);
lcd_data("AIR POLLUTION");
lcd_command(0xc0);
lcd_data("CONTROL SYS");
lcd_command(0x01);
lcd_delay(100000);
lcd_delay(60000); lcd_delay(60000);lcd_delay(60000);
  lcd_delay(60000); lcd_delay(60000);lcd_delay(60000);
lcd_command(0x88);
while(1)
{
sensor_s();
if((IO1PIN & (1<<16)) ==0)      //CO2 sensor digital input function//
{
co=co+1;
time(1000);
lcd_delay(60000);
lcd_command(0x01);
lcd_command(0x80);
lcd_data("time:");
lcd_putval(co);
    lcd_data("S");
time(500);
if(co==1)
{
lcd_data(" Co2 Warning ");
lcd_command(0xc0);
lcd_data("MSG Sending......");
sms_alert();
lcd_delay(60000); lcd_delay(60000);lcd_delay(60000);
lcd_command(0x01);
}
if(co>10)
{
IOCLR1=0x80000000;
IOSET1=0x10000000;
IOSET1=0x40000000;
lcd_command(0x01);
lcd_command(0x80);
lcd_data(" Co2 HIGH ");
lcd_command(0xc0);
lcd_data("MSG Sending......");
sms_alert1();
lcd_delay(60000); lcd_delay(60000);lcd_delay(60000);
lcd_command(0x01);
co=0;
}
else
{
IOCLR1=0x10000000;
IOCLR1=0x40000000;
IOSET1=0x80000000;
}
 

}
}
}
