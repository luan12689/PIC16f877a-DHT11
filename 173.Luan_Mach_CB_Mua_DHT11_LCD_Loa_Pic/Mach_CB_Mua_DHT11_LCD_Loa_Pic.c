#include <16F877a.h>       
#fuses      HS                  
#use delay(clock=20000000)    

#define LCD_ENABLE_PIN  PIN_b2                                    
#define LCD_RS_PIN      PIN_b4                                   
#define LCD_RW_PIN      PIN_b3                                    

#define LCD_DATA4       PIN_d7                                    
#define LCD_DATA5       PIN_d6                                    
#define LCD_DATA6       PIN_d5                                    
#define LCD_DATA7       PIN_d4

#include <lcd.c>
 
#define     loa         pin_d0         
#define     led         pin_c2    
#define     mua         pin_d1        

#BIT  DHT_DATA_OUT =  0x05.0        // chan dht11 gan vao vdk 
#BIT  DHT_DATA_IN  =  0x05.0        // PORTA . bit so 0
#BIT  DHT_DDR_DATA =  0x85.0

#define DHT_ER    0
#define DHT_OK    1

#define DDROUT    0
#define DDRIN     1

UNSIGNED int8 ND,ND1,DA,DA1;


VOID setup()      
{
   SET_TRIS_A(0Xff);       // khoi tao port di vao vdk
   SET_TRIS_e(0Xff);
   SET_TRIS_d(0X00);
   set_tris_b(0x00);   
   set_tris_c(0x00);   
   
   lcd_init();               
   output_low(loa);           
   output_low(led);     
}
//unsigned int8 DHT_GetTemHumi (unsigned int8 &tem1,&tem2,unsigned int8 &humi1,&humi2);

unsigned int8 DHT_GetTemHumi(unsigned int8 &tem1,&tem2,unsigned int8 &humi1,&humi2)
{
   unsigned int8 buffer[5]={0,0,0,0,0};
   unsigned int8 ii,i,checksum;
      
   DHT_DDR_DATA=DDROUT;   // set la cong ra, huong vao hay ra
   DHT_DATA_OUT=1;
   delay_us(60);
   DHT_DATA_OUT=0;
   delay_ms(25); // it nhat 18ms
   DHT_DATA_OUT=1;    
   DHT_DDR_DATA=DDRIN;//set la cong vao
   delay_us(60);
   if(DHT_DATA_IN)return DHT_ER ;
   else while(!(DHT_DATA_IN));   //Doi DaTa len 1
   delay_us(60);//cho 60us
   if(!DHT_DATA_IN)return DHT_ER;
   else while((DHT_DATA_IN));    //Doi Data ve 0
   //Bat dau doc du lieu
   for(i=0;i<5;i++)
   {
      for(ii=0;ii<8;ii++)
      {   
         while((!DHT_DATA_IN));//Doi Data len 1
         delay_us(50);
         if(DHT_DATA_IN)
         {
            buffer[i]|=(1<<(7-ii));
            while((DHT_DATA_IN));//Doi Data xuong 0
         }
      }
   }
   checksum=buffer[0]+buffer[1]+buffer[2]+buffer[3];
   if((checksum)!=buffer[4])return DHT_ER;
   tem1  =   buffer[2];//gia tri phan nguyen
   tem2  =   buffer[3];//gia tri phan thap phan
   humi1 =   buffer[0];//gia tri phan nguyen
   humi2 =   buffer[1];//gia tri phan thap phan
   return DHT_OK;
}

void doc_dht11()
{
    if(DHT_GetTemHumi(ND,ND1,DA,DA1)==1)
    {
       lcd_gotoxy(1,1);           
       lcd_putc("ND:");
       lcd_putc(ND/10+0X30);        
       lcd_putc(ND%10+0X30);           
       lcd_putc(0XDF);
       lcd_putc("C    ");
       
       lcd_gotoxy(10,1);            
       lcd_putc("DA:");
       lcd_putc(DA/10+0X30);
       lcd_putc(DA%10+0X30);
       lcd_putc(0X25);              // %    
    }
    else 
    {
    lcd_gotoxy(1,1);
    lcd_putc(" KHONG GAN CB ");
    }
    
    
}

void kiemtra()
{
   if(input(mua)==0)    
   {
      output_high(loa);       
      output_high(led);
      delay_ms(100);
      output_low(led);
      output_low(loa);
      delay_ms(100);
      lcd_gotoxy(1,2);              
      lcd_putc("  TROI DANG MUA  ");
   }
   else           
   {
      output_low(loa);
      output_low(led);
      lcd_gotoxy(1,2);             
      lcd_putc(" TROI KHONG MUA ");
      delay_ms(200);
   }
}

void main()
{
   setup();      
   while(true)
   {    
      doc_dht11();
      kiemtra();
   }
}
/***************************************************************/

