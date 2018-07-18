#ifndef  __HS_RC523_API_H__
#define  __HS_RC523_API_H__


#ifdef  __cplusplus
    extern "C" {
#endif


#include "Nano100Series.h"


typedef enum {
    RC523_MEM_BLOCK_0 = 0,    // 不可写, 只能读
    RC523_MEM_BLOCK_1,
    RC523_MEM_BLOCK_2,
    RC523_MEM_BLOCK_3,        // 千万不要写此块
    RC523_MEM_BLOCK_4,
    RC523_MEM_BLOCK_5,
    RC523_MEM_BLOCK_6,
    RC523_MEM_BLOCK_7,        // 千万不要写此块
    RC523_MEM_BLOCK_8,
    RC523_MEM_BLOCK_9,
    RC523_MEM_BLOCK_10,
    RC523_MEM_BLOCK_11,      // 千万不要写此块
    RC523_MEM_BLOCK_12,
    RC523_MEM_BLOCK_13,
    RC523_MEM_BLOCK_14,
    RC523_MEM_BLOCK_15,     // 千万不要写此块
    RC523_MEM_BLOCK_16,
    RC523_MEM_BLOCK_17,
    RC523_MEM_BLOCK_18,
    RC523_MEM_BLOCK_19,     // 千万不要写此块
    RC523_MEM_BLOCK_20,
    RC523_MEM_BLOCK_21,
    RC523_MEM_BLOCK_22,
    RC523_MEM_BLOCK_23,     // 千万不要写此块
    RC523_MEM_BLOCK_24,
    RC523_MEM_BLOCK_25,
    RC523_MEM_BLOCK_26,
    RC523_MEM_BLOCK_27,     // 千万不要写此块
    RC523_MEM_BLOCK_28,
    RC523_MEM_BLOCK_29,
    RC523_MEM_BLOCK_30,
    RC523_MEM_BLOCK_31,     // 千万不要写此块
    RC523_MEM_BLOCK_32,
    RC523_MEM_BLOCK_33,
    RC523_MEM_BLOCK_34,
    RC523_MEM_BLOCK_35,     // 千万不要写此块
    RC523_MEM_BLOCK_36,
    RC523_MEM_BLOCK_37,
    RC523_MEM_BLOCK_38,
    RC523_MEM_BLOCK_39,     // 千万不要写此块
    RC523_MEM_BLOCK_40,
    RC523_MEM_BLOCK_41,
    RC523_MEM_BLOCK_42,
    RC523_MEM_BLOCK_43,     // 千万不要写此块
    RC523_MEM_BLOCK_44,
    RC523_MEM_BLOCK_45,
    RC523_MEM_BLOCK_46,
    RC523_MEM_BLOCK_47,     // 千万不要写此块
    RC523_MEM_BLOCK_48,
    RC523_MEM_BLOCK_49,
    RC523_MEM_BLOCK_50,
    RC523_MEM_BLOCK_51,     // 千万不要写此块
    RC523_MEM_BLOCK_52,
    RC523_MEM_BLOCK_53,
    RC523_MEM_BLOCK_54,
    RC523_MEM_BLOCK_55,      // 千万不要写此块
    RC523_MEM_BLOCK_56,
    RC523_MEM_BLOCK_57,
    RC523_MEM_BLOCK_58,
    RC523_MEM_BLOCK_59,      // 千万不要写此块
    RC523_MEM_BLOCK_60,
    RC523_MEM_BLOCK_61,
    RC523_MEM_BLOCK_62,
    RC523_MEM_BLOCK_63       // 千万不要写此块
} RC523_Mem_Block;

typedef enum {
    RC523_CARD_TYPE_A = 'A',
    RC523_CARD_TYPE_B = 'B'
} RC523_Card_Type;


uint8_t hs_rc523_init(void);

uint8_t hs_rc523_release(void);

int8_t hs_rc523_config(RC523_Card_Type cardType);

void hs_hex_2_string(uint8_t *in, uint16_t len, uint8_t *out);

void hs_string_2_hex(uint8_t *in, uint16_t len, uint8_t *out);

int8_t hs_rc523_get_card_id(RC523_Card_Type cardType, uint8_t *id);

int8_t hs_rc523_read_block(RC523_Mem_Block block, uint8_t *data, uint8_t *cardID);

int8_t hs_rc523_write_block(RC523_Mem_Block block, uint8_t *data);


#ifdef  __cplusplus
}
#endif

#endif
