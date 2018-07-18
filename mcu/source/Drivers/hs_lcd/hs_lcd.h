#ifndef  __HS_LCD_H__
#define  __HS_LCD_H__


#include "Nano100Series.h"


typedef struct  
{										    
    unsigned short  width;		//LCD 宽度
    unsigned short  height;		//LCD 高度
    unsigned short  id;	                //LCD ID
    unsigned char   dir;	        //横屏还是竖屏控制：0，竖屏；1，横屏。	
    unsigned short  touch_x;		//触摸点横坐标
    unsigned short  touch_y;		//触摸点纵坐标
}_lcd_dev; 


extern _lcd_dev lcd_dev;


void hs_lcd_init(void);
void hs_ILI9806e_init(void);

void hs_lcd_write_data(uint8_t ucData);

void hs_lcd_write_command(uint8_t ucCommand);

unsigned char hs_lcd_read_reg(uint8_t ucRegAddr);


#endif