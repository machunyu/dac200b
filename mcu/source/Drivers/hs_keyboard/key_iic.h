#ifndef _KEY_IIC_H_
#define _KEY_IIC_H_

#define IIC_ACK_OK              0
#define IIC_ACK_NO              1

#define KEY_IIC_SCL             PA11
#define KEY_IIC_SCL_PORT        PA
#define KEY_IIC_SCL_Pin         BIT11

#define KEY_IIC_SDA             PA10
#define KEY_IIC_SDA_PORT        PA
#define KEY_IIC_SDA_Pin         BIT10

#define Set_IIC_SCL		(KEY_IIC_SCL = 1)
#define Clr_IIC_SCL		(KEY_IIC_SCL = 0)

#define Set_IIC_SDA		(KEY_IIC_SDA = 1)
#define Clr_IIC_SDA		(KEY_IIC_SDA = 0)

#define Get_IIC_SDA		(KEY_IIC_SDA)

#define IIC_SDA_IN()            (GPIO_SetMode(KEY_IIC_SDA_PORT, KEY_IIC_SDA_Pin, GPIO_PMD_INPUT))
#define IIC_SDA_OUT()           (GPIO_SetMode(KEY_IIC_SDA_PORT, KEY_IIC_SDA_Pin, GPIO_PMD_OUTPUT))


extern void IIC_Start(void);
extern void IIC_Stop(void);
extern uint8_t IIC_WriteByte(uint8_t DataBuf);
extern uint8_t IIC_ReadByte(uint8_t ACK);

#endif