
#include <stdio.h>
#include <string.h>
#include "key_led.h"
#include "hs_delay.h"
#include "hs_timer.h"
#include "hs_iwdg.h"
#include "hs_beep.h"
#include "hs_led.h"
#include "hs_key.h"
#include "hs_door.h"
#include "hs_power.h"
#include "hs_usart.h"
#include "hs_detect.h"
#include "hs_queue.h"
#include "hs_flash.h"
#include "hs_keyboard.h"
#include "hs_rc523_api.h"
#include "hs_systick.h"
#include "hs_timer_list.h"
#include "hs_test.h"
#include "hs_card_manager.h"
#include "hs_module.h"
#include "hs_type.h"
#include "hs_config.h"


void hs_beep_test(void)
{
    hs_beep_init();
    
    while(1) {
        hs_beep_toggle();
        //hs_beep_on();
        hs_delay_ms(200);
        
        //hs_beep_off();
        //hs_delay_ms(100);
    }
}

void hs_debug_led_test(void)
{
    hs_led_init();
    
    while(1) {
        //hs_led_on();
        hs_led_toggle(LED_0);
        hs_led_toggle(LED_1);
        hs_delay_ms(1000);
        
        //hs_led_off();
        //hs_delay_ms(104);
    }
}

void hs_detect_test(void)
{
    uint16_t usVoltage;
    
    hs_beep_init();
    hs_detect_init();
    
    while(1) {
        usVoltage = hs_detect_get_voltage();
        printf("------> usVoltage = %d mv\r\n", usVoltage);
        if(usVoltage >= THRESHOLD_VAL) {
            hs_beep_on();
            hs_delay_ms(200);
            hs_beep_off();
        }
        
        hs_delay_ms(100);
    }
}

static void hs_make_data(uint8_t *buf, uint32_t len, uint16_t max)
{
    uint32_t i;
    
    for(i=0; i<len; i++) {
        buf[i] = i % max;
    }
    
    return;
}

uint8_t hs_memcmp(uint8_t *data1, uint8_t *data2, uint32_t size)
{
    uint8_t ret = 0;
    uint32_t i = 0;
    
    for(i=0; i<size; i++) {
        if(*data1++ != *data2++) {
            ret = 1;
            break;
        }
    }
    
    return ret;
}

uint8_t rx_buf[4000];
uint8_t tx_buf[4000];
    
void hs_flash_test(void)
{
    uint32_t ret = 0;
    uint8_t data = 0xff;
    //uint8_t rx_buf[100];
    //uint8_t tx_buf[100];
    
    printf("==================== Flash Test =====================\r\n\r\n");
    
    ret = hs_flash_init();
    printf("----------> flash init %s \r\n\r\n", ret ? "Failed" : "Succeed");
    
    
    while(1) {
        hs_make_data(tx_buf, 4000, data--);
      
        printf("----------> flash write : \r\n");
        hs_print_message(tx_buf, 4000);
        printf("\r\n =============================================\r\n\r\n");
        hs_flash_write(HS_CFG_CARD_A_ADDR, tx_buf, 4000);

        printf("----------> flash read : \r\n");
        hs_flash_read(HS_CFG_CARD_A_ADDR, rx_buf, 4000);
        hs_print_message(rx_buf, 4000);
        printf("\r\n =============================================\r\n\r\n");
        
        ret = hs_memcmp(rx_buf, tx_buf, 4000);
        if( !ret ) {
            printf("----------> verify data succeed ... \r\n");
        } else {
            printf("----------> verify data failed ... \r\n");
        }
    }
    
    hs_make_data(tx_buf, 100, 16);
    printf("----------> flash write : \r\n");
    hs_flash_write(HS_CFG_CARD_A_ADDR, tx_buf, 100);
    
    printf("----------> flash read : \r\n");
    hs_flash_read(HS_CFG_CARD_A_ADDR, rx_buf, 100);
    
    ret = hs_memcmp(rx_buf, tx_buf, 100);
    if( !ret ) {
        printf("----------> verify data succeed ... \r\n");
    } else {
        printf("----------> verify data failed ... \r\n");
    }

    memset(tx_buf, 0xFF, 100);
    ret = hs_flash_erase(HS_CFG_CARD_A_ADDR/512, 1);
    printf("----------> flash erase %s \r\n\r\n", ret ? "Failed" : "Succeed");
    
    printf("----------> flash read : \r\n");
    hs_flash_read(HS_CFG_CARD_A_ADDR, rx_buf, 100);

    ret = hs_memcmp(rx_buf, tx_buf, 100);
    if( !ret ) {
        printf("----------> verify data succeed ... \r\n");
    } else {
        printf("----------> verify data failed ... \r\n");
    }
    
    while(1);
}

void hs_iwdg_test(void)
{
    hs_led_init();
    hs_iwdg_init(WDT_PERIOD_1741_MS);
    //hs_delay_ms(20);
    //hs_iwdg_reset(WDT_PERIOD_512_MS);
    
    if(WDT_GET_RESET_FLAG()) {
        WDT_CLEAR_RESET_FLAG();

        hs_led_toggle(LED_0);
        hs_delay_ms(200);
        hs_led_toggle(LED_0);
    }
    
    while(1){
        hs_led_toggle(LED_0);
        hs_delay_ms(200);
        
        hs_iwdg_feet();
    }
}

void hs_key_test(void)
{
    uint32_t ulCurTicks = 0;
    uint32_t ulStartTicks = 0;
    uint32_t ulPressTime = 0;
    uint8_t ucKeyFalg = 0;
    
    hs_led_init();
    hs_key_init();

    
    while(1) {
        hs_delay_ms(20);
        
        if( !hs_key_read_pin() ) {
            ulStartTicks = hs_systick_get_tick();
            
            while( !hs_key_read_pin() ) {
                hs_delay_ms(50);
                
                ulCurTicks = hs_systick_get_tick();
                ulPressTime += (ulCurTicks - ulStartTicks) * 20;
                ulStartTicks = ulCurTicks;
                
                if( ulPressTime >= 3000 ) {
                    ulPressTime = 0;
                    ucKeyFalg = 1;
                    break;
                }
            }
            
            if(ucKeyFalg) {
                ucKeyFalg = 0;
                hs_led_toggle(LED_0);
            }
            
            while( !hs_key_read_pin() );
        }
        
    }
}

void hs_key_led_test(void)
{
    uint8_t i = 0;
    uint8_t LEDs[13] = "0123456789*#";

    hs_key_led_init();
    hs_key_led_all_on();
    hs_iwdg_delay_ms(1000);
    hs_key_led_all_off();
    hs_iwdg_delay_ms(1000);

#if 0
    
    while(1) {
        for(i=0; i<12; i++) {
            hs_key_led_toggle(LEDs[i]);
            hs_iwdg_delay_ms(1000);
        }
    }
    
#else
    
    while(1) {
        for(i=0; i<6; i++) {
            hs_key_leds_on("123");
            hs_iwdg_delay_ms(1000);
            
            hs_key_leds_on("456");
            hs_iwdg_delay_ms(1000);
            
            hs_key_leds_on("789");
            hs_iwdg_delay_ms(1000);
            
            hs_key_leds_on("*0#");
            hs_iwdg_delay_ms(1000);
            
            hs_key_led_all_on();
            hs_iwdg_delay_ms(1000);
        }
    }
    
#endif
}

void hs_keyboard_test(void)
{
    uint8_t ucKeyVal, ret = 0;
    
    hs_key_led_init();
    hs_keyboard_init();
    
    printf("============== keyboard test ================\r\n");
    ret = hs_keyboard_config();
    if(ret) {
        printf("Failed to hs_keyboard_config ...\r\n");
    }

    while(1) {
        hs_delay_ms(20);
        ucKeyVal = hs_keyboard_get();
        if((ucKeyVal != 0xFF) && (ucKeyVal != 0xEE)) {
            hs_key_led_on(ucKeyVal);
            hs_delay_ms(100);
            hs_key_led_off(ucKeyVal);
        }
    }
}

void hs_power_test(void)
{
    hs_power_init();
    
    while(1) {
        hs_a33_power_on();
        hs_iris_on();
        hs_delay_ms(3000);

        hs_a33_power_off();
        hs_iris_off();
        hs_delay_ms(3000);
    }
}

#if 1
void hs_rc523_test(void)
{
    int8_t ret = 0, IDLen = 4;
    uint8_t i, j, cardID[8] = {0}, IDString[20];
    uint8_t rx_data_block1[3][16] = {0};
    uint8_t cnt = 0, swichFlag = 0;
    RC523_Card_Type CurCardType = RC523_CARD_TYPE_A;

    hs_rc523_init();

    printf("=================== Rc523 Test ====================\r\n");

    while(1) {
    	if(RC523_CARD_TYPE_A == CurCardType && swichFlag == 1) {
    	    hs_rc523_config(RC523_CARD_TYPE_B);
            CurCardType = RC523_CARD_TYPE_B;
            IDLen = 8;
    	    swichFlag = 0;
    	    printf(">> Plese Put Type B Card \r\n");
    	    
    	} else if(RC523_CARD_TYPE_B == CurCardType && swichFlag == 1){
    	    hs_rc523_config(RC523_CARD_TYPE_A);
            CurCardType = RC523_CARD_TYPE_A;
            IDLen = 4;
    	    swichFlag = 0;
    	    printf(">> Plese Put Type A Card \r\n");
    	}

    	ret = hs_rc523_get_card_id(CurCardType, cardID);
    	if(!ret) {
            memset(IDString, 0, sizeof(IDString));
            hs_hex_2_string(cardID, IDLen, IDString);
    	    printf(" -----> Get card id : %s\r\n", IDString);
            if(CurCardType == RC523_CARD_TYPE_A) {
                ret  = hs_rc523_read_block(RC523_MEM_BLOCK_4, rx_data_block1[0], NULL);
                ret |= hs_rc523_read_block(RC523_MEM_BLOCK_5, rx_data_block1[1], NULL);
                ret |= hs_rc523_read_block(RC523_MEM_BLOCK_6, rx_data_block1[2], NULL);
                if(!ret) {
    	            printf("=======================================================\r\n");
    	            printf("--------> read block : \r\n");
                    for(i=0; i<3; i++) {
                        for(j=0; j<16; j++) {
                            printf("   %x", rx_data_block1[i][j]);
                        }
                        printf("\r\n");
                    }
    	            printf("=======================================================\r\n\r\n");
                }
            }
    	    memset(cardID, 0, sizeof(cardID));
            memset(rx_data_block1, 0, 48);
    	    cnt ++;
    	    if(cnt >= 3) {
    	    	cnt = 0;
    	    	swichFlag = 1;
    	    }
    	}

    	hs_delay_ms(100);
    }
}

#else 

void hs_rc523_test(void) 
{
    int8_t ret = 0;
    uint8_t i, j, cardID[20] = {0};
    uint8_t rx_data_block1[3][16] = {0};
    RC523_Mem_Block block = RC523_MEM_BLOCK_1;
    uint8_t tx_data[16] = { 
        0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 
        0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
    };
    
    hs_rc523_init();
    
    printf("=================== rc523 test ====================\r\n");
           
    while(1) {
        printf("Put your card A to write block \r\n");
        ret |= hs_rc523_write_block(RC523_MEM_BLOCK_4, tx_data);
        ret |= hs_rc523_write_block(RC523_MEM_BLOCK_5, tx_data);
        ret |= hs_rc523_write_block(RC523_MEM_BLOCK_6, tx_data);
        if(!ret) {
            printf("=======================================================\r\n");
            printf("--------> write block %d ok \r\n\r\n", block);
        } else {
            printf("---------> fail to find card \r\n\r\n");
        }
        
        hs_delay_ms(1000);
    
        printf("Put your card A to read block \r\n");
        memset(cardID, 0, 10);
        memset(rx_data_block1, 0, 48);
        ret  = hs_rc523_read_block(RC523_MEM_BLOCK_4, rx_data_block1[0], cardID);
        ret |= hs_rc523_read_block(RC523_MEM_BLOCK_5, rx_data_block1[1], cardID);
        ret |= hs_rc523_read_block(RC523_MEM_BLOCK_6, rx_data_block1[2], cardID);
        if(!ret) {
    	    printf("=======================================================\r\n");
    	    printf("--------> card ID   : %s\r\n", cardID);
    	    printf("--------> read block : \r\n");
            for(i=0; i<3; i++) {
                for(j=0; j<16; j++) {
                    printf("   %x", rx_data_block1[i][j]);
                }
                printf("\r\n");
            }
    	    printf("=======================================================\r\n\r\n");
        } else {
            printf("---------> fail to find card \r\n");
        }
    	
        hs_delay_ms(1000);
    }
}

#endif

void hs_usart_test(void)
{
    uint8_t string[] = "Hello World\r\n";
    
    hs_usart_init();
    
    while(1) {
        hs_usart_send_string(string);
        hs_delay_ms(1000);
    }
}

void hs_timer_list_test(void)
{
    hs_beep_init();
    hs_door_init();
    hs_key_led_init();
    hs_timer_list_init();
    hs_led_init();
    
#if 0
    while(1) {
        hs_beep_on();
        hs_key_led_off('0');
        hs_timer_node_run(KEYBOARD_TIMER_ID, 3, '0');
        hs_timer_node_run(BEEP_TIMER_ID, 3, 0);
        
        hs_delay_ms(1000);
    }
#endif
    
#if 0
    while(1) {
        hs_door_open(METHOD_RELAY);
        hs_timer_node_run(CLSOE_TIMER_ID, 1, METHOD_RELAY);
        
        hs_delay_ms(5000);
    }
#endif
    
#if 1
    while(1) {
        hs_timer_node_run(ERROR_TIMER_ID, 0, LED_0);
        
        hs_delay_ms(1000);
    }
#endif
}

void hs_timer1_test(void)
{
    hs_key_led_init();
    hs_power_init();
    hs_timer1_init();
    
    hs_timer1_start();
    
    while(1);
}
