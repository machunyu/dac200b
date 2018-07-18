#include <intrinsics.h>
#include "Nano100Series.h"
#include "hs_delay.h"
#include "key_iic.h"

static void _disableInterrupt(void)
{
    __disable_interrupt();
}

static void _enableInterrupt(void)
{
    __enable_interrupt();
}

static void iic_delay_us(int t)
{
    hs_delay_us(t);
}

void IIC_Start(void)
{
    IIC_SDA_OUT();

    Set_IIC_SDA;
    Set_IIC_SCL;

    iic_delay_us(5);

    Clr_IIC_SDA;
    iic_delay_us(5);
    Clr_IIC_SCL;
    iic_delay_us(5);
}

void IIC_Stop(void)
{
    IIC_SDA_OUT();

    Clr_IIC_SDA;
    iic_delay_us(5);
    Set_IIC_SCL;
    iic_delay_us(5);
    Set_IIC_SDA;
    iic_delay_us(5);
}

uint8_t IIC_WriteByte(uint8_t DataBuf)
{
    uint8_t Temp, Data = DataBuf;
    IIC_SDA_OUT();
_disableInterrupt();
    for(Temp = 0;Temp < 8;Temp++) {
        if(Data & 0x80)
            Set_IIC_SDA;
        else
            Clr_IIC_SDA;
        iic_delay_us(5);
        Set_IIC_SCL;
        iic_delay_us(5);
        Clr_IIC_SCL;
        Data <<= 1;
    }
    IIC_SDA_IN();
    iic_delay_us(5);
    Set_IIC_SCL;
    iic_delay_us(5);
    Temp = Get_IIC_SDA;
    Clr_IIC_SCL;
    iic_delay_us(5);
_enableInterrupt();
    if(Temp)
        return 1;
    else
        return 0;
}

uint8_t IIC_ReadByte(uint8_t ACK)
{
    uint8_t Temp, Data = 0;
    IIC_SDA_IN();
_disableInterrupt();
    for(Temp = 0;Temp < 8;Temp++) {
        Data <<= 1;

        Set_IIC_SCL;
        iic_delay_us(5);

        if(Get_IIC_SDA)
            Data |= 0x01;

        Clr_IIC_SCL;
        iic_delay_us(5);
    }

    IIC_SDA_OUT();

    if(ACK)
        Clr_IIC_SDA;
    else
        Set_IIC_SDA;
    iic_delay_us(5);
    Set_IIC_SCL;
    iic_delay_us(5);
    Clr_IIC_SCL;
    iic_delay_us(5);
_enableInterrupt();
    return Data;
}