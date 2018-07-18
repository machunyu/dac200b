#include <stdio.h>
#include <string.h>
#include "hs_type.h"
#include "hs_usart.h"
#include "hs_msg_process.h"

RxFrame_t  RxFrame;

void hs_usart_init(void)
{
    ///* Unlock protected registers */
    //SYS_UnlockReg();
    //
    ///* Enable IP clock */
    //CLK_EnableModuleClock(UART0_MODULE);
    //
    ///* Select IP clock source */
    //CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART_S_HXT, CLK_UART_CLK_DIVIDER(1));
    //
    ///* Set PA multi-function pins for UART1 RXD and TXD */
    //SYS->PA_H_MFP &= ~( SYS_PA_H_MFP_PA14_MFP_Msk | SYS_PA_H_MFP_PA15_MFP_Msk);
    //SYS->PA_H_MFP |= (SYS_PA_H_MFP_PA15_MFP_UART0_TX | SYS_PA_H_MFP_PA14_MFP_UART0_RX);
    //
    ///* Lock protected registers */
    //SYS_LockReg();

    /* Init UART to 115200-8n1 for print message */
    UART_Open(UART0, 115200);
    
    UART_EnableInt(UART0, UART_IER_RDA_IE_Msk);
    
    NVIC_EnableIRQ(UART0_IRQn);
}

void hs_usart_send_char(uint8_t data)
{
    while(UART0->FSR & UART_FSR_TX_FULL_F_Msk);
    UART0->THR = data;
}

void hs_usart_send_string(uint8_t *string)
{
    while(*string) hs_usart_send_char(*string++);
}

void hs_usart_send_data(uint8_t *data, uint16_t len)
{
    while(len--)  hs_usart_send_char(*data++);
}

void hs_RxFrame_clear(void)
{
    memset(&RxFrame, 0, sizeof(RxFrame));
}

void hs_RxFrame_insert_data(uint8_t data)
{
    RxFrame.Frame[RxFrame.FrameLen] = data;
    RxFrame.FrameLen = (RxFrame.FrameLen + 1) % FRAME_BUF_LEN;
}

void hs_RxFrame_update_rx_state(void)
{
    uint16_t FrameLen = RxFrame.FrameLen;
    uint16_t FrameRecvLen = RxFrame.FrameRecvLen;
    
    if( RxFrame.FrameState < RX_STATE_F_HEAD ) {
        if( RxFrame.Frame[0] == 0xAA ) {
            RxFrame.FrameState = RX_STATE_F_HEAD;
        } else {
            RxFrame.FrameLen = 0;
        }
    } else if( (RxFrame.FrameState == RX_STATE_F_HEAD) && (RxFrame.FrameLen > 3) ) {
        RxFrame.FrameState = RX_STATE_F_LEN;
        RxFrame.FrameRecvLen = RxFrame.Frame[2];
        RxFrame.FrameRecvLen <<= 8;
        RxFrame.FrameRecvLen |= RxFrame.Frame[3];
    } else if( (RxFrame.FrameState == RX_STATE_F_LEN) && (FrameLen >= FrameRecvLen) ) {
        //hs_print_message(RxFrame.Frame, RxFrame.FrameLen);
        if(hs_check_frame(&RxFrame)) {
            if(Conv_16_BIT(RxFrame.m_Frame.command) < 0x2000) {
                // handle uart recv
                hs_msg_int_process(Conv_16_BIT(RxFrame.m_Frame.command), RxFrame.m_Frame.dat, Conv_16_BIT(RxFrame.m_Frame.dat_len));
                hs_RxFrame_clear();
            } else {
                RxFrame.FrameState = RX_STATE_F_FINISH;
            }
        } else {
             hs_RxFrame_clear();
        }
    }
}

RxState_t hs_RxFrame_get_state(void)
{
    return RxFrame.FrameState;
}

uint16_t hs_RxFrame_read_data(uint8_t *data)
{
    uint16_t usFrameLen = 0;
    
    if(RxFrame.FrameLen) {
        memcpy(data, RxFrame.Frame, RxFrame.FrameLen);
        usFrameLen = RxFrame.FrameLen;
        hs_RxFrame_clear();
    }
    
    return usFrameLen;
}

void hs_print_message(uint8_t *msg, uint16_t len)
{
    uint16_t i = 0;
    
    while(len--) {
        hs_printf(INFO, "%x ", msg[i++]);
    }
    
    return;
}



