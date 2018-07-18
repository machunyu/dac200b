#ifndef _HS_SYS_TYPE_H_
#define _HS_SYS_TYPE_H_

#include <stdio.h>

/*
   HS_VERSION is (major << 16) + (minor << 8) + patch.
*/
#define HS_HAL_HW_VERSION       0x010300
#define HS_HAL_SW_VERSION       0x030200

/*
   can be used like #if (HS_VERSION >= HS_VERSION_CHECK(1, 0, 0))
*/
#define HS_VERSION_CHECK(major, minor, patch) ((major<<16)|(minor<<8)|(patch))
#define HS_PACKAGEDATE_STR      "2018-5-25"


#define Conv_16_BIT(val)  ((val&0x00ff)<<8)+((val&0xff00)>>8)


#define HS_DEBUG_LEVEL (HS_DEBUG_INFO|HS_DEBUG_WARN|HS_DEBUG_ERROR)
#define HS_DEBUG_OFF   0x00
#define HS_DEBUG_INFO  0x01
#define HS_DEBUG_WARN  0x02
#define HS_DEBUG_ERROR 0x04

//#define HS_DBG(format,...)  printf("File: "__FILE__", Line: %05d: "format"/n", __LINE__, ##__VA_ARGS__)  

#define HS_DBG_INFO(msg...)  do { if(HS_DEBUG_LEVEL & HS_DEBUG_INFO)  printf(msg);}while(0)
#define HS_DBG_WARN(msg...)  do { if(HS_DEBUG_LEVEL & HS_DEBUG_WARN)  printf(msg);}while(0)
#define HS_DBG_ERROR(msg...) do { if(HS_DEBUG_LEVEL & HS_DEBUG_ERROR) printf(msg);}while(0)
#define hs_printf(level, msg...) HS_DBG_##level(msg)

#endif
