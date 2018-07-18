#include <stdio.h>
#include <string.h>
#include "main.h"


uint8_t hs_module_init(void)
{
    uint8_t ret = 0;

    hs_delay_init();
    hs_systick_init();
    hs_debug_init();
    hs_usart_init();
    hs_beep_init();
    hs_key_init();
    hs_door_init();
    hs_detect_init();
    hs_led_init();
    hs_a33_io_init();
    hs_key_led_init();
    hs_keyboard_init();
    hs_power_init();
    hs_timer1_init();
   
    hs_rc523_init();

    hs_beep_on();
    hs_key_led_all_on(); 
    hs_delay_ms(200);
    hs_beep_off();
    hs_key_led_all_off(); 
    hs_delay_ms(800);
    hs_timer1_start();                                                                                                     
    hs_a33_power_on();
    hs_delay_ms(1000);
    hs_lcd_init();
    //hs_ILI9806e_init();
    
    hs_iwdg_init(WDT_PERIOD_1741_MS);
     
    return ret;
}

uint8_t hs_module_later_init(void)
{
    uint8_t ret = 0;
    uint32_t ulTemp;
    uint32_t ulResetCnt = 0;

    hs_heart_beator_init();
    
    ret |= hs_flash_init();
    hs_delay_ms(200);
    ret |= hs_keyboard_probe();
    ret |= hs_sys_info_init();
    ret |= hs_card_manager_init();
    ret |= hs_timer_list_init();
    if( !ret ) {
        hs_door_close(g_SysInfo.ucOpenMethod);
    }
    
    ret = hs_flash_read(HS_CFG_UPDATE_INFO_ADDR, (uint8_t*)&ulTemp, 4);
    if( !ret ) {
        if(ulTemp == HS_CFG_UPDATE_INFO) {
            g_stFlag.ucUpdateFlag = 1;
        } else {
            g_stFlag.ucUpdateFlag = 0;
        }

        hs_printf(INFO, " read UpdateFlag : %x\r\n", ulTemp);
    } else {
        hs_printf(ERROR, "read UpdateFlag error\r\n");
    }
    
    if( WDT_GET_RESET_FLAG() ) {
        WDT_CLEAR_RESET_FLAG();
        hs_flash_read(0, (uint8_t*)&ulResetCnt, 4);
        ulResetCnt += 1;
        hs_flash_write(HS_CFG_RESET_CNT_ADDR, (uint8_t*)&ulResetCnt, 4);
        hs_printf(INFO, " mcu is reset by iwdg && reset times : %ld \r\n", ulResetCnt);
    } else {
        ulResetCnt = 0;
        hs_flash_write(HS_CFG_RESET_CNT_ADDR, (uint8_t*)&ulResetCnt, 4);
    }
    
    return ret;
}

uint8_t hs_module_probe(void)
{
    return hs_keyboard_probe();
}

uint8_t hs_module_suspend(void)
{
    return 0;
}

uint8_t hs_module_resume(void)
{
    return 0;
}

uint8_t hs_module_reset(void)
{
    uint8_t ret = 0;
    AppTimer_t data;
    uint32_t ulResetCnt = 0;
    uint32_t ulUpdateFlag = HS_CFG_UNUPDATE_INFO;

    ret = hs_flash_erase(0, HS_CFG_DATA_FLASH_PAGES);
    if( !ret ) {
        memset(&data, 0, sizeof(data));
        data.ulReloadTime = HS_CFG_DEFAULT_OPEN_PERIOD * 1000 / TIMER_PERIOD_MS;
        hs_timer_node_update(CLSOE_TIMER_ID, &data);

        g_SysInfo.ucLockMode = HS_CFG_DEFAULT_LOCK_MODE;
        g_SysInfo.ucOpenMethod = HS_CFG_DEFAULT_OPEN_METHOD;
        g_SysInfo.ucOpenHoldPeriod = HS_CFG_DEFAULT_OPEN_PERIOD;
        g_SysInfo.ucIrisPowCtl = 0;
        g_SysInfo.ulCardACapacity = HS_CFG_CARD_A_DEFAULT_CAP;
        g_SysInfo.ulCardBCapacity = HS_CFG_CARD_B_DEFAULT_CAP;
        g_SysInfo.usHwVersion = hs_sys_get_hw_version();
        g_SysInfo.usSwVersion = hs_sys_get_sw_version();
        memset(&g_SysInfo.sDate, 0, sizeof(Date_t));

        ret |= hs_flash_write(HS_CFG_RESET_CNT_ADDR, (uint8_t*)&ulResetCnt, 4);
        ret |= hs_flash_write(HS_CFG_UPDATE_INFO_ADDR, (uint8_t*)&ulUpdateFlag, 4);
        ret |= hs_flash_write(HS_CFG_SYS_INFO_ADDR, (uint8_t*)&g_SysInfo, sizeof(SysCfgTypeDef));
    }
    
    return ret;
}

uint8_t hs_module_sleep(void)
{
    hs_beep_off();
    hs_key_led_all_off();
    hs_set_sys_state(SYS_STATE_SLEEP);
    
    return 0;
}

uint8_t hs_module_wakeup(void)
{
    hs_set_sys_state(SYS_STATE_WAKEUP);
    hs_key_led_start_up();
    hs_beep_on();
    hs_timer_node_run(BEEP_TIMER_ID, 4, 0);
    
    return 0;
}

uint8_t hs_module_response_key(uint8_t keyVal)
{
    hs_beep_on();
    hs_key_led_off(keyVal);
    
    hs_timer_node_run(KEYBOARD_TIMER_ID, 3, keyVal);
    hs_timer_node_run(BEEP_TIMER_ID, 3, 0);
    
    return 0;
}

uint8_t hs_module_status_cback(Module_Status_t status)
{
    return 0;
}
