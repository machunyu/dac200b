
#include "hs_delay.h"
#include "key_iic.h"
#include "hs_keyboard.h"

#define CH_SENS(a,b)       0x##a##b
#define CURSENS            CH_SENS(B,B)


static uint8_t TSM_Write_Byte(uint8_t Addr, uint8_t Data)
{
    IIC_Start();
    if( IIC_WriteByte(DEVICE_WRITE_ADDR) == IIC_ACK_OK ) {
        if( IIC_WriteByte(Addr) == IIC_ACK_OK ) {
            if( IIC_WriteByte(Data) == IIC_ACK_OK ) {
                IIC_Stop();
                return 1;
            }
        }
    }
    IIC_Stop();
    return 0;
}

static uint8_t TSM_Read_Str(uint8_t Addr, uint8_t *Buf, uint8_t Len)
{
    uint8_t i;
    Len--;
    IIC_Start();
    if( IIC_WriteByte(DEVICE_WRITE_ADDR) == IIC_ACK_OK ) {
        if( IIC_WriteByte(Addr) == IIC_ACK_OK ) {
            IIC_Start();
            if(IIC_WriteByte(DEVICE_READ_ADDR) == IIC_ACK_OK ) {
                for(i = 0; i<Len; i++) {
                    *Buf++ = IIC_ReadByte(1);
                }
                *Buf = IIC_ReadByte(0);
                IIC_Stop();
                return 1;
            }
        }
    }
    IIC_Stop();
    return 0;
}

static uint8_t TSM12_Port_Init(void)
{
    GPIO_SetMode(PA, BIT10 | BIT11 , GPIO_PMD_OUTPUT);
    GPIO_SetMode(PB, BIT12, GPIO_PMD_OUTPUT);
    
    GPIO_ENABLE_PULL_UP(PA, BIT10 | BIT11);
    GPIO_ENABLE_PULL_UP(PB, BIT12);
    
#if ENABLE_INT
    GPIO_SetMode(PB, BIT14, GPIO_PMD_INPUT);
    GPIO_ENABLE_PULL_UP(PB, BIT14);
    
    GPIO_EnableInt(PB, 14, GPIO_INT_FALLING);
    NVIC_EnableIRQ(KEY_EXTI_IRQn);
#else 
    GPIO_SetMode(PB, BIT14, GPIO_PMD_OUTPUT);
    PB14 = 1;
#endif
    
    PA10  = 1;
    PA11 = 1;
    PB12 = 1;
    
    return 0;
}

uint8_t hs_keyboard_config(void)
{
    uint8_t ret = 0;

    KEY_IIC_ENABLE();
    hs_delay_ms(10);
    if(TSM_Write_Byte(CTRL2, 0x0f) == 0) ret = 1;
    hs_delay_ms(10);
    if(TSM_Write_Byte(CTRL2, 0x07) == 0) ret = 1;
    //delay_ms(100);
    
    if(TSM_Write_Byte(SENS1, CURSENS) == 0) ret = 1;
    if(TSM_Write_Byte(SENS2, CURSENS) == 0) ret = 1;
    if(TSM_Write_Byte(SENS3, CURSENS) == 0) ret = 1;
    if(TSM_Write_Byte(SENS4, CURSENS) == 0) ret = 1;
    if(TSM_Write_Byte(SENS5, CURSENS) == 0) ret = 1;
    if(TSM_Write_Byte(SENS6, CURSENS) == 0) ret = 1;

    if(TSM_Write_Byte(CTRL1, 0x31) == 0) ret = 1;
    if(TSM_Write_Byte(REF_RST1, 0x00) == 0) ret = 1;
    if(TSM_Write_Byte(REF_RST2, 0x00) == 0) ret = 1;
    if(TSM_Write_Byte(CH_HOLD1, 0x00) == 0) ret = 1;
    if(TSM_Write_Byte(CH_HOLD2, 0x00) == 0) ret = 1;
  
    KEY_IIC_DISABLE();

    return ret;
}

uint8_t hs_keyboard_init(void)
{
    TSM12_Port_Init();

    return 0; //hs_keyboard_config();
}

uint8_t hs_keyboard_release(void)
{
    return 1;
}

uint8_t hs_keyboard_probe(void)
{
    uint8_t index;

    for(index = 0; index < 3; index++) {
        if(!hs_keyboard_config()) {
            return 0;
        }
    }
    return 1;
}

uint8_t hs_keyboard_get(void)
{
  uint32_t temp = 0;
  
  union {
    uint32_t Key;
    uint8_t KeyBuff[4];
  } NewKey;
  
  static uint32_t KeyVal = 0;

  KEY_IIC_ENABLE();
  NewKey.Key = 0;
  hs_delay_ms(1);
  TSM_Read_Str(OUTPUT1, NewKey.KeyBuff, 3);
  KEY_IIC_DISABLE();

  if(NewKey.Key)
  {
    NewKey.Key &= 0x00aaaaaa;
    temp = NewKey.Key & (NewKey.Key ^ KeyVal);
    KeyVal |= NewKey.Key;

    switch(temp)
    {
      case 0x00080000:
        return '*';
      case 0x00800000:
        return '#';
      case 0x00008000:
        return '8';
      case 0x00002000:
        return '7';
      case 0x00000002:
        return '1';
      case 0x00020000:
        return '9';
      case 0x00000800:
        return '6';
      case 0x00000020:
        return '3';
      case 0x00200000:
        return '0';
      case 0x00000200:
        return '5';
      case 0x00000008:
        return '2';
      case 0x00000080:
        return '4';
      default:
        return 0xee;
    }
  }
  else
  {
    KeyVal = 0;
  }

  return 0xff;
}

uint8_t TSM12_Into_Sleep_Mode(void)
{
    uint8_t ret;
    
    KEY_IIC_ENABLE();
    hs_delay_ms(1);
    if(TSM_Write_Byte(CTRL2, 0x07) == 0) ret = 0;
    else ret = 1;
    KEY_IIC_DISABLE();
    return ret;
}

uint8_t TSM12_Exit_Sleep_Mode(void)
{
    uint8_t ret;
    
    KEY_IIC_ENABLE();
    hs_delay_ms(100);
    if(TSM_Write_Byte(CTRL2, 0x03) == 0) ret = 0;
    else ret = 1;
    KEY_IIC_DISABLE();
    return ret;
}

