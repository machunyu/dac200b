#ifndef _HS_TSM12_H_
#define _HS_TSM12_H_

#include "key_led.h"

#define DEVICE_WRITE_ADDR       0xD0
#define DEVICE_READ_ADDR        0xD1

#define SENS1                   0x02    //灵敏度设置
#define SENS2                   0x03
#define SENS3                   0x04
#define SENS4                   0x05
#define SENS5                   0x06
#define SENS6                   0x07
#define CTRL1                   0x08    //模式控制.上电效验时间.中断输出.响应周期
#define CTRL2                   0x09    //软件复位.休眠模式控制
#define REF_RST1                0x0A    //参考更新
#define REF_RST2                0x0B    
#define CH_HOLD1                0x0C    //工作时自动校准
#define CH_HOLD2                0x0D
#define CAL_HOLD1               0x0E    //
#define CAL_HOLD2               0x0F
#define OUTPUT1                 0x10
#define OUTPUT2                 0x11
#define OUTPUT3                 0x12

#define KEY_IIC_EN              PB12
#define KEY_IIC_EN_PORT         PB
#define KEY_IIC_EN_Pin          BIT12

#define KEY_INT                 PB14
#define KEY_INT_PORT            PB
#define KEY_INT_Pin             BIT14
#define KEY_EXTI_IRQn           GPABC_IRQn

#define KEY_IIC_ENABLE()        (KEY_IIC_EN = 0)
#define KEY_IIC_DISABLE()       (KEY_IIC_EN = 1)

#define Get_KEY_OUT()           (KEY_INT)

#define ENABLE_INT              0


extern uint8_t hs_keyboard_init(void);
extern uint8_t hs_keyboard_config(void);
extern uint8_t hs_keyboard_probe(void);
extern uint8_t hs_keyboard_release(void);
extern uint8_t hs_keyboard_get(void);
extern uint8_t TSM12_Into_Sleep_Mode(void);
extern uint8_t TSM12_Exit_Sleep_Mode(void);


#endif
