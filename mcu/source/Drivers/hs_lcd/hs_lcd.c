#include "hs_lcd.h"
#include "hs_delay.h"


#define LCD_RESET              PC10
#define LCD_RESET_PIN          PC
#define LCD_RESET_PORT         BIT10

#define LCD_CS                 PC8
#define LCD_CS_PIN             PC
#define LCD_CS_PORT            BIT8

#define LCD_CLK                PC9
#define LCD_CLK_PIN            PC
#define LCD_CLK_PORT           BIT9

#define LCD_MO                 PC11
#define LCD_MO_PIN             PC
#define LCD_MO_PORT            BIT11

#define LCD_MO_OUTPUT()          GPIO_SetMode(PC, BIT11, GPIO_PMD_OUTPUT)
#define LCD_MO_INPUT()           GPIO_SetMode(PC, BIT11, GPIO_PMD_INPUT)

#define LCD_READ_MO_PIN        LCD_MO


static void hs_lcd_delay_ms(uint32_t ucPeriodInMs)
{
    hs_delay_ms(ucPeriodInMs);
}

static void LCD_WRITE_CS_PIN(uint8_t ucPinVal)
{
    if(ucPinVal) {
        LCD_CS = 1;
    } else {
        LCD_CS = 0;
    }
}

static void LCD_WRITE_CLK_PIN(uint8_t ucPinVal)
{
    if(ucPinVal) {
        LCD_CLK = 1;
    } else {
        LCD_CLK = 0;
    }
}

static void LCD_WRITE_MO_PIN(uint8_t ucPinVal)
{
    if(ucPinVal) {
        LCD_MO = 1;
    } else {
        LCD_MO = 0;
    }
}

static void hs_lcd_gpio_init(void)
{
    GPIO_SetMode(PC, BIT8 | BIT9 | BIT10 | BIT11, GPIO_PMD_OUTPUT);
}

_lcd_dev lcd_dev;

void hs_lcd_init(void)
{
    hs_lcd_gpio_init();

    //LCD_WRITE_RESET_PIN(1);
    //hs_lcd_delay_ms(1);
    //LCD_WRITE_RESET_PIN(0);
    //hs_lcd_delay_ms(10);
    //LCD_WRITE_RESET_PIN(1);
    hs_lcd_delay_ms(200);

#if 0
    hs_lcd_write_command(0xd3); 
    lcd_dev.id = hs_lcd_read_data();  //dummy read
    lcd_dev.id = hs_lcd_read_data();  //读到0x00
    lcd_dev.id = hs_lcd_read_data();  //读到0x98

    lcd_dev.id <<=8;
    lcd_dev.id  |= hs_lcd_read_data();  //读到0x06
#endif
    
    //*************************//LCD SETING
    hs_lcd_write_command(0xFF); // EXTC Command Set enable register
    hs_lcd_write_data(0xFF);
    hs_lcd_write_data(0x98);
    hs_lcd_write_data(0x06); 

    hs_lcd_write_command(0xBA);// SPI Interface Setting
    hs_lcd_write_data(0xE0); 

    /////GIP Timing Setting for //
    hs_lcd_write_command(0xBC);//GPI
    hs_lcd_write_data(0x03);
    hs_lcd_write_data(0x0E);
    hs_lcd_write_data(0x03);
    hs_lcd_write_data(0x63);
    hs_lcd_write_data(0x01);
    hs_lcd_write_data(0x01);
    hs_lcd_write_data(0x1B);
    hs_lcd_write_data(0x12);
    hs_lcd_write_data(0x39);
    hs_lcd_write_data(0x00);
    hs_lcd_write_data(0x00);
    hs_lcd_write_data(0x00);
    hs_lcd_write_data(0x01);
    hs_lcd_write_data(0x01);
    hs_lcd_write_data(0x02);
    hs_lcd_write_data(0x02);
    hs_lcd_write_data(0xFF);
    hs_lcd_write_data(0xF2);
    hs_lcd_write_data(0x01);
    hs_lcd_write_data(0x00);
    hs_lcd_write_data(0xC0); 
           
    hs_lcd_write_command(0xBD);//GPI
    hs_lcd_write_data(0x02);
    hs_lcd_write_data(0x13);
    hs_lcd_write_data(0x45);
    hs_lcd_write_data(0x67);
    hs_lcd_write_data(0x45);
    hs_lcd_write_data(0x67);
    hs_lcd_write_data(0x01);
    hs_lcd_write_data(0x23);  
           
    hs_lcd_write_command(0xBE);//GPI
    hs_lcd_write_data(0x01);
    hs_lcd_write_data(0x22);
    hs_lcd_write_data(0x22);
    hs_lcd_write_data(0xDC);
    hs_lcd_write_data(0xBA);
    hs_lcd_write_data(0x67);
    hs_lcd_write_data(0x22);
    hs_lcd_write_data(0x22);
    hs_lcd_write_data(0x22);
     
    hs_lcd_write_command(0xC7);
    hs_lcd_write_data(0X78); //VCOM

    hs_lcd_write_command(0xED);// en_volt_reg measure VGMP
    hs_lcd_write_data(0x7F);
    hs_lcd_write_data(0x0F);
    hs_lcd_write_data(0x00);
           
    hs_lcd_write_command(0xC0);// Power Control 1
    hs_lcd_write_data(0x03);
    hs_lcd_write_data(0x0B);
    hs_lcd_write_data(0x0A);
     
    hs_lcd_write_command(0xF5);//Select AVDD regulator voltage level
    hs_lcd_write_data(0x20);
    hs_lcd_write_data(0x43);
    hs_lcd_write_data(0x00);
     
    hs_lcd_write_command(0xEE);// GIP 3
    hs_lcd_write_data(0x0A);
    hs_lcd_write_data(0x1B);
    hs_lcd_write_data(0x5F);
    hs_lcd_write_data(0x40);
    hs_lcd_write_data(0x28);
    hs_lcd_write_data(0x38);
    hs_lcd_write_data(0x02);
    hs_lcd_write_data(0x2B);
    hs_lcd_write_data(0x50);
    hs_lcd_write_data(0x00);
    hs_lcd_write_data(0x80);
           
    hs_lcd_write_command(0xFC);//LVGL Voltage Setting
    hs_lcd_write_data(0x08);
           
    hs_lcd_write_command(0xDF);// GIP 3
    hs_lcd_write_data(0x00);
    hs_lcd_write_data(0x00);
    hs_lcd_write_data(0x00);
    hs_lcd_write_data(0x00);
    hs_lcd_write_data(0x00);
    hs_lcd_write_data(0x20);
           
    hs_lcd_write_command(0xF3);// DVDD Voltage Setting
    hs_lcd_write_data(0x74);
           
    hs_lcd_write_command(0xB4);
    hs_lcd_write_data(0x02); // 2DOT inversion 
    hs_lcd_write_data(0x02);// 2DOT inversion 
    hs_lcd_write_data(0x02);// 2DOT inversion 
           
    hs_lcd_write_command(0xF7);
    hs_lcd_write_data(0x82); //480x800
           
    hs_lcd_write_command(0xB1);// Frame Rate Control
    hs_lcd_write_data(0x00);
    hs_lcd_write_data(0x13);//70.39
    hs_lcd_write_data(0x13);//70.39
           
    hs_lcd_write_command(0xF2);//Panel Timing Control
    hs_lcd_write_data(0x89);
    hs_lcd_write_data(0x92);
    hs_lcd_write_data(0x52);
    hs_lcd_write_data(0x28);
           
    hs_lcd_write_command(0xF9);//Panel Timing Control
    hs_lcd_write_data(0x00);
    hs_lcd_write_data(0xFD);
    hs_lcd_write_data(0x80);
    hs_lcd_write_data(0xF0);
    hs_lcd_write_data(0xF0);
           
    hs_lcd_write_command(0xC1);
    hs_lcd_write_data(0x17);//Source OP Amp bias control.
    hs_lcd_write_data(0x75);//Sets the VGMP voltage for positive Gamma 
    hs_lcd_write_data(0x65);//Sets the VGMN voltage for negative Gamma 
    hs_lcd_write_data(0x20);

    hs_lcd_write_command(0xE0);//GAMMA
    hs_lcd_write_data(0x00);
    hs_lcd_write_data(0x04);
    hs_lcd_write_data(0x0D);
    hs_lcd_write_data(0x0F);
    hs_lcd_write_data(0x11);
    hs_lcd_write_data(0x19);
    hs_lcd_write_data(0xC9);
    hs_lcd_write_data(0x08);
    hs_lcd_write_data(0x01);
    hs_lcd_write_data(0x06);
    hs_lcd_write_data(0x06);
    hs_lcd_write_data(0x0B);
    hs_lcd_write_data(0x0D);
    hs_lcd_write_data(0x31);
    hs_lcd_write_data(0x2C);
    hs_lcd_write_data(0x00);

    hs_lcd_write_command(0xE1);//GAMMA
    hs_lcd_write_data(0x00);
    hs_lcd_write_data(0x05);
    hs_lcd_write_data(0x08);
    hs_lcd_write_data(0x0D);
    hs_lcd_write_data(0x10);
    hs_lcd_write_data(0x12);
    hs_lcd_write_data(0x79);
    hs_lcd_write_data(0x08);
    hs_lcd_write_data(0x03);
    hs_lcd_write_data(0x08);
    hs_lcd_write_data(0x09);
    hs_lcd_write_data(0x0E);
    hs_lcd_write_data(0x0E);
    hs_lcd_write_data(0x2A);
    hs_lcd_write_data(0x22);
    hs_lcd_write_data(0x00);
    ////////////////////////////////
    
    hs_lcd_write_command(0x55);//CE
    hs_lcd_write_data(0x90); 
           
    hs_lcd_write_command(0x3A);
    hs_lcd_write_data(0x77); //55-16BIT,66-18BIT,77-24BIT
           
    //**************SET RGB interface mode*******************
    hs_lcd_write_command(0xB6);
    hs_lcd_write_data(0x20); //SET RGB DPI
           
    hs_lcd_write_command(0xB0);
    hs_lcd_write_data(0x03); //02，此寄存器设RGB时序极性，具休参考IC规格书和主控来调整.

#if 0
    hs_lcd_write_command(0x36);
    hs_lcd_write_data(0x00);
    
    //********地址窗口2A 2B **********
    hs_lcd_write_command(0x2A); //480
    hs_lcd_write_data(0x00);
    hs_lcd_write_data(0x00);
    hs_lcd_write_data(0x01);
    hs_lcd_write_data(0xDF);
           
    hs_lcd_write_command(0x2B); //800
    hs_lcd_write_data(0x00);
    hs_lcd_write_data(0x00);
    hs_lcd_write_data(0x03);
    hs_lcd_write_data(0x1f);
    //*****************************
#else
    
    hs_lcd_write_command(0x36);
    hs_lcd_write_data(0xA0); // 0x60
    
    //********地址窗口2A 2B **********
    hs_lcd_write_command(0x2A); //480
    hs_lcd_write_data(0x00);
    hs_lcd_write_data(0x00);
    hs_lcd_write_data(0x03);
    hs_lcd_write_data(0x1F);
           
    hs_lcd_write_command(0x2B); //800
    hs_lcd_write_data(0x00);
    hs_lcd_write_data(0x00);
    hs_lcd_write_data(0x01);
    hs_lcd_write_data(0xDf);
    //*****************************
#endif
    hs_lcd_write_command(0x11);
    hs_lcd_delay_ms(120);
    hs_lcd_write_command(0x29);
    hs_lcd_delay_ms(25);
    hs_lcd_write_command(0x2C);
}

void hs_ILI9806e_init(void)
{
    hs_lcd_gpio_init();
    hs_lcd_delay_ms(10);
    //***************************************************************//LCD SETING
    hs_lcd_write_command(0xFF);        // Change to Page 1 CMD 
    hs_lcd_write_data(0xFF); 
    hs_lcd_write_data(0x98); 
    hs_lcd_write_data(0x06); 
    hs_lcd_write_data(0x04); 
    hs_lcd_write_data(0x01); 
     
    hs_lcd_write_command(0x08);        //Output    SDA 
    hs_lcd_write_data(0x10); 
    
    hs_lcd_write_command(0x20);        //set DE/VSYNC mode    平台注意
    hs_lcd_write_data(0x00); 	
    
    hs_lcd_write_command(0x21);        //DE = 1 Active 
    hs_lcd_write_data(0x03); 
     
    hs_lcd_write_command(0x30);        //Resolution setting 480 X 800 
    hs_lcd_write_data(0x02); 
     
    hs_lcd_write_command(0x31);        //Inversion setting 2-dot 
    hs_lcd_write_data(0x02); 
     
    hs_lcd_write_command(0x3A);      
    hs_lcd_write_data(0x70); //24BIT
     
    hs_lcd_write_command(0x60);       
    hs_lcd_write_data(0x07); 
     
    hs_lcd_write_command(0x61);        
    hs_lcd_write_data(0x06); 
     
    hs_lcd_write_command(0x62);      
    hs_lcd_write_data(0x06); 
     
    hs_lcd_write_command(0x63);      
    hs_lcd_write_data(0x04); 
    
    hs_lcd_write_command(0x40); //BT  AVDD,AVDD 倍压
    hs_lcd_write_data(0x18); //10,14
     
    hs_lcd_write_command(0x41);//AVDD，AVEE设置
    hs_lcd_write_data(0x33); //avdd +5.4v,avee-5.2v ;55
     
    hs_lcd_write_command(0x42);  //VGL ,VGH  SET  
    hs_lcd_write_data(0x11); 
     
    hs_lcd_write_command(0x43);//Default 
    hs_lcd_write_data(0x09); 
     
    hs_lcd_write_command(0x44);       
    hs_lcd_write_data(0x0C); 
    
    hs_lcd_write_command(0x46); //增加    
    hs_lcd_write_data(0x55);
     
    hs_lcd_write_command(0x47); //增加     
    hs_lcd_write_data(0x55); 
    
    hs_lcd_write_command(0x45);       
    hs_lcd_write_data(0x14); 
     
    hs_lcd_write_command(0x50);        //VREG1 
    hs_lcd_write_data(0x50); 
     
    hs_lcd_write_command(0x51);        //VREG2 
    hs_lcd_write_data(0x50); 
     
    hs_lcd_write_command(0x52);        //Flicker MSB 
    hs_lcd_write_data(0x00); 
     
    hs_lcd_write_command(0x53);        //Flicker LSB 
    hs_lcd_write_data(0x38); //VCOM
    
    hs_lcd_write_command(0xA0);        //Positive Gamma 
    hs_lcd_write_data(0x00); 
    hs_lcd_write_command(0xA1);        // 
    hs_lcd_write_data(0x09); 
    hs_lcd_write_command(0xA2);        // 
    hs_lcd_write_data(0x0C); 
    hs_lcd_write_command(0xA3);        // 
    hs_lcd_write_data(0x0F); 
    hs_lcd_write_command(0xA4);        // 
    hs_lcd_write_data(0x06); 
    hs_lcd_write_command(0xA5);        // 
    hs_lcd_write_data(0x09); 
    hs_lcd_write_command(0xA6);        // 
    hs_lcd_write_data(0x07); 
    hs_lcd_write_command(0xA7);        // 
    hs_lcd_write_data(0x16); 
    hs_lcd_write_command(0xA8);        // 
    hs_lcd_write_data(0x06); 
    hs_lcd_write_command(0xA9);        // 
    hs_lcd_write_data(0x09); 
    hs_lcd_write_command(0xAA);        // 
    hs_lcd_write_data(0x11); 
    hs_lcd_write_command(0xAB);        // 
    hs_lcd_write_data(0x06); 
    hs_lcd_write_command(0xAC);        // 
    hs_lcd_write_data(0x0E); 
    hs_lcd_write_command(0xAD);        // 
    hs_lcd_write_data(0x19); 
    hs_lcd_write_command(0xAE);        // 
    hs_lcd_write_data(0x0E); 
    hs_lcd_write_command(0xAF);        // 
    hs_lcd_write_data(0x00); 
     
    hs_lcd_write_command(0xC0);        //Negative Gamma 
    hs_lcd_write_data(0x00); 
    hs_lcd_write_command(0xC1);        // 
    hs_lcd_write_data(0x09); 
    hs_lcd_write_command(0xC2);        // 
    hs_lcd_write_data(0x0C); 
    hs_lcd_write_command(0xC3);        // 
    hs_lcd_write_data(0x0F); 
    hs_lcd_write_command(0xC4);        // 
    hs_lcd_write_data(0x06); 
    hs_lcd_write_command(0xC5);        // 
    hs_lcd_write_data(0x09); 
    hs_lcd_write_command(0xC6);        // 
    hs_lcd_write_data(0x07); 
    hs_lcd_write_command(0xC7);        // 
    hs_lcd_write_data(0x16); 
    hs_lcd_write_command(0xC8);        // 
    hs_lcd_write_data(0x06); 
    hs_lcd_write_command(0xC9);        // 
    hs_lcd_write_data(0x09); 
    hs_lcd_write_command(0xCA);        // 
    hs_lcd_write_data(0x11); 
    hs_lcd_write_command(0xCB);        // 
    hs_lcd_write_data(0x06); 
    hs_lcd_write_command(0xCC);        // 
    hs_lcd_write_data(0x0E); 
    hs_lcd_write_command(0xCD);        // 
    hs_lcd_write_data(0x19); 
    hs_lcd_write_command(0xCE);        // 
    hs_lcd_write_data(0x0E); 
    hs_lcd_write_command(0xCF);        // 
    hs_lcd_write_data(0x00); 
     
    
    hs_lcd_write_command(0xFF);        // Change to Page 6 CMD for GIP timing   
    hs_lcd_write_data(0xFF); 
    hs_lcd_write_data(0x98); 
    hs_lcd_write_data(0x06); 
    hs_lcd_write_data(0x04); 
    hs_lcd_write_data(0x06); 
    
    hs_lcd_write_command(0x00);        // 
    hs_lcd_write_data(0xA0); 
    hs_lcd_write_command(0x01);        // 
    hs_lcd_write_data(0x05); 
    hs_lcd_write_command(0x02);        // 
    hs_lcd_write_data(0x00); 
    hs_lcd_write_command(0x03);        // 
    hs_lcd_write_data(0x00); 
    hs_lcd_write_command(0x04);        // 
    hs_lcd_write_data(0x01); 
    hs_lcd_write_command(0x05);        // 
    hs_lcd_write_data(0x01); 
    hs_lcd_write_command(0x06);        // 
    hs_lcd_write_data(0x88); 
    hs_lcd_write_command(0x07);        // 
    hs_lcd_write_data(0x04); 
    hs_lcd_write_command(0x08);        // 
    hs_lcd_write_data(0x01); 
    hs_lcd_write_command(0x09);        // 
    hs_lcd_write_data(0x90); 
    hs_lcd_write_command(0x0A);        // 
    hs_lcd_write_data(0x04); 
    hs_lcd_write_command(0x0B);        // 
    hs_lcd_write_data(0x01); 
    hs_lcd_write_command(0x0C);        // 
    hs_lcd_write_data(0x01); 
    hs_lcd_write_command(0x0D);        // 
    hs_lcd_write_data(0x01); 
    hs_lcd_write_command(0x0E);        // 
    hs_lcd_write_data(0x00); 
    hs_lcd_write_command(0x0F);        // 
    hs_lcd_write_data(0x00);
    
    hs_lcd_write_command(0x10);        // 
    hs_lcd_write_data(0x55); 
    hs_lcd_write_command(0x11);        // 
    hs_lcd_write_data(0x50); 
    hs_lcd_write_command(0x12);        // 
    hs_lcd_write_data(0x01); 
    hs_lcd_write_command(0x13);        // 
    hs_lcd_write_data(0x85); 
    hs_lcd_write_command(0x14);        // 
    hs_lcd_write_data(0x85); 
    hs_lcd_write_command(0x15);        // 
    hs_lcd_write_data(0xC0); 
    hs_lcd_write_command(0x16);        // 
    hs_lcd_write_data(0x0B); 
    hs_lcd_write_command(0x17);        // 
    hs_lcd_write_data(0x00); 
    hs_lcd_write_command(0x18);        // 
    hs_lcd_write_data(0x00); 
    hs_lcd_write_command(0x19);        // 
    hs_lcd_write_data(0x00); 
    hs_lcd_write_command(0x1A);        // 
    hs_lcd_write_data(0x00); 
    hs_lcd_write_command(0x1B);        // 
    hs_lcd_write_data(0x00); 
    hs_lcd_write_command(0x1C);        // 
    hs_lcd_write_data(0x00); 
    hs_lcd_write_command(0x1D);        // 
    hs_lcd_write_data(0x00); 
    
    hs_lcd_write_command(0x20);        // 
    hs_lcd_write_data(0x01); 
    hs_lcd_write_command(0x21);        // 
    hs_lcd_write_data(0x23); 
    hs_lcd_write_command(0x22);        // 
    hs_lcd_write_data(0x45); 
    hs_lcd_write_command(0x23);        // 
    hs_lcd_write_data(0x67); 
    hs_lcd_write_command(0x24);        // 
    hs_lcd_write_data(0x01); 
    hs_lcd_write_command(0x25);        // 
    hs_lcd_write_data(0x23); 
    hs_lcd_write_command(0x26);        // 
    hs_lcd_write_data(0x45); 
    hs_lcd_write_command(0x27);        // 
    hs_lcd_write_data(0x67); 
    
    hs_lcd_write_command(0x30);        // 
    hs_lcd_write_data(0x02); 
    hs_lcd_write_command(0x31);        // 
    hs_lcd_write_data(0x22); 
    hs_lcd_write_command(0x32);        // 
    hs_lcd_write_data(0x11); 
    hs_lcd_write_command(0x33);        // 
    hs_lcd_write_data(0xAA); 
    hs_lcd_write_command(0x34);        // 
    hs_lcd_write_data(0xBB); 
    hs_lcd_write_command(0x35);        // 
    hs_lcd_write_data(0x66); 
    hs_lcd_write_command(0x36);        // 
    hs_lcd_write_data(0x03); 
    hs_lcd_write_command(0x37);        // 
    hs_lcd_write_data(0x22); 
    hs_lcd_write_command(0x38);        // 
    hs_lcd_write_data(0x22); 
    hs_lcd_write_command(0x39);        // 
    hs_lcd_write_data(0x22); 
    hs_lcd_write_command(0x3A);        // 
    hs_lcd_write_data(0x22); 
    hs_lcd_write_command(0x3B);        // 
    hs_lcd_write_data(0x22); 
    hs_lcd_write_command(0x3C);        // 
    hs_lcd_write_data(0x22); 
    hs_lcd_write_command(0x3D);        // 
    hs_lcd_write_data(0x22); 
    hs_lcd_write_command(0x3E);        // 
    hs_lcd_write_data(0x22); 
    hs_lcd_write_command(0x3F);        // 
    hs_lcd_write_data(0x22); 
    hs_lcd_write_command(0x40);        // 
    hs_lcd_write_data(0x22); 
    hs_lcd_write_command(0x52);        // 
    hs_lcd_write_data(0x12); 
    hs_lcd_write_command(0x53);        // 
    hs_lcd_write_data(0x12); 
    
    hs_lcd_write_command(0xFF);        // Change to Page 7 CMD for GIP timing   
    hs_lcd_write_data(0xFF); 
    hs_lcd_write_data(0x98); 
    hs_lcd_write_data(0x06); 
    hs_lcd_write_data(0x04); 
    hs_lcd_write_data(0x07); 
     
    hs_lcd_write_command(0x17);        // 
    hs_lcd_write_data(0x32); 
    
    hs_lcd_write_command(0x02);        // 
    hs_lcd_write_data(0x17); 
    
    hs_lcd_write_command(0x18);        // 
    hs_lcd_write_data(0x1D); 
    
    hs_lcd_write_command(0xE1);        // 
    hs_lcd_write_data(0x79); 
    
    hs_lcd_write_command(0xFF);        // Change to Page 7 CMD for Normal command 
    hs_lcd_write_data(0xFF); 
    hs_lcd_write_data(0x98); 
    hs_lcd_write_data(0x06); 
    hs_lcd_write_data(0x04); 
    hs_lcd_write_data(0x00); 
    
    
    hs_lcd_write_command(0x11);
    hs_lcd_delay_ms(120);
    hs_lcd_write_command(0x29);
    hs_lcd_delay_ms(25);
    hs_lcd_write_command(0x2C);
}


void hs_lcd_write_data(uint8_t ucData)
{
    uint8_t i;

    LCD_WRITE_CS_PIN(0);
    LCD_WRITE_CLK_PIN(0);
    LCD_WRITE_MO_PIN(1);
    LCD_WRITE_CLK_PIN(1);
    for(i=0; i<8; i++) {
        LCD_WRITE_CLK_PIN(0);
        
        if (ucData & 0x80) {
            LCD_WRITE_MO_PIN(1);
        } else {
            LCD_WRITE_MO_PIN(0);
        }
        
        LCD_WRITE_CLK_PIN(1);
        ucData <<= 1;
    }
    
    LCD_WRITE_CS_PIN(1);
}

void hs_lcd_write_command(uint8_t ucCommand)
{
    uint8_t i;
    
    LCD_WRITE_CS_PIN(0);
    LCD_WRITE_CLK_PIN(0);
    LCD_WRITE_MO_PIN(0);
    LCD_WRITE_CLK_PIN(1);
    for(i=0; i<8; i++) {
        LCD_WRITE_CLK_PIN(0);   
        
        if (ucCommand & 0x80) {
            LCD_WRITE_MO_PIN(1);
        } else {
            LCD_WRITE_MO_PIN(0);
        }
        
        LCD_WRITE_CLK_PIN(1);
        ucCommand <<= 1;
    }
    
    LCD_WRITE_CS_PIN(1);
}

unsigned char hs_lcd_read_reg(uint8_t ucRegAddr)
{
    uint8_t i, ucRegData = 0;

    LCD_WRITE_CS_PIN(0);
    LCD_WRITE_CLK_PIN(0);
    LCD_WRITE_MO_PIN(1);
    LCD_WRITE_CLK_PIN(1);
    for(i=0; i<8; i++) {
        LCD_WRITE_CLK_PIN(0);
        
        if (ucRegAddr & 0x80) {
            LCD_WRITE_MO_PIN(1);
        } else {
            LCD_WRITE_MO_PIN(0);
        }
        
        LCD_WRITE_CLK_PIN(1);
        ucRegAddr <<= 1;
    }
    
    LCD_WRITE_CLK_PIN(1);
    LCD_WRITE_MO_PIN(1);
    LCD_MO_INPUT();
    
    for(i=0; i<8; i++) {
       LCD_WRITE_CLK_PIN(0);
       
       if(LCD_READ_MO_PIN) {
           ucRegData |= 1;
       }
       
       LCD_WRITE_CLK_PIN(1);
    }
    
    LCD_WRITE_CS_PIN(1);
    LCD_MO_OUTPUT();
    
    return 0;
}
