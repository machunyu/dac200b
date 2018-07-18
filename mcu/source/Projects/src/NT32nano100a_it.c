#include "NT32nano100a_it.h"
#include "main.h"


void GPABC_IRQHandler(void)
{
    /* To check if PB.5 interrupt occurred */
    if (PB->ISRC & BIT14) {
        PB->ISRC = BIT14;
    } else {
        /* Un-expected interrupt. Just clear all PORTA, PORTB, PORTC interrupts */
        PA->ISRC = PA->ISRC;
        PB->ISRC = PB->ISRC;
        PC->ISRC = PC->ISRC;
    }
}

void TMR0_IRQHandler(void)
{
    // clear timer interrupt flag
    TIMER_ClearIntFlag(TIMER0);

    hs_timer_callback();
}

extern uint8_t g_ucInitFalg;
void TMR1_IRQHandler(void)
{
    uint8_t cnt = 0;
    uint32_t ulRunTimeMs = 0;
    
    // clear timer interrupt flag
    TIMER_ClearIntFlag(TIMER1);
    
    g_nTicks++;
    ulRunTimeMs = g_nTicks * 50;
    cnt = (ulRunTimeMs / 200) % 4;
    
    if(0 == cnt) {
        hs_key_leds_on("1");
    } else if(1 == cnt) {
        hs_key_leds_on("12");
    } else if(2 == cnt) {
        hs_key_leds_on("123");
    } else {
       hs_key_led_all_off();
    }

    if(ulRunTimeMs >= 6000 && g_ucInitFalg == 0) {
        g_ucInitFalg = 1;
    }
    
    if(ulRunTimeMs == 10000) {
        if(!g_SysInfo.ucIrisPowCtl) {
            hs_iris_on();
        }
    }
}

void UART0_IRQHandler(void)
{
    uint8_t ucData;
    uint32_t ulIntStatus = UART0->ISR;
#if 1
    if( ulIntStatus & UART_ISR_RDA_IS_Msk ) {
        /* Get all the input characters */
        while( !UART_GET_RX_EMPTY(UART0) ) {
            /* Get the character from UART Buffer */
            ucData = UART_READ(UART0);  /* Rx trigger level is 1 byte */
            if( hs_RxFrame_get_state() != RX_STATE_F_FINISH ) {
                hs_RxFrame_insert_data(ucData);
                hs_RxFrame_update_rx_state();
                //hs_printf(INFO, "%x ", ucData);
            }
        }
    }
#endif
}

void SysTick_Handler(void)
{
    hs_systick_inc_ticks();
    
    hs_systick_callback();
}
