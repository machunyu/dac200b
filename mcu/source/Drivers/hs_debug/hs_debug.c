#include "hs_debug.h"


void hs_debug_init(void)
{
    /* Unlock protected registers */
    //SYS_UnlockReg();
    //
    ///* Enable IP clock */
    //CLK_EnableModuleClock(UART1_MODULE);
    //
    ///* Select IP clock source */
    //CLK_SetModuleClock(UART1_MODULE, CLK_CLKSEL1_UART_S_HXT, CLK_UART_CLK_DIVIDER(1));
    //
    ///* Set PA multi-function pins for UART1 RXD and TXD */
    //SYS->PA_L_MFP &= ~( SYS_PA_L_MFP_PA2_MFP_Msk | SYS_PA_L_MFP_PA3_MFP_Msk);
    //SYS->PA_L_MFP |= (SYS_PA_L_MFP_PA3_MFP_UART1_TX|SYS_PA_L_MFP_PA2_MFP_UART1_RX);
    //
    ///* Lock protected registers */
    //SYS_LockReg();
    
    /* Init UART to 115200-8n1 for print message */
    UART_Open(UART1, 115200);
}

