#include <stdio.h>
#include <string.h>
#include "hs_delay.h"
#include "hs_queue.h"
#include "hs_usart.h"


#define QUEUE_BUF_LEN     1024


typedef struct {
    uint16_t qFront;
    uint16_t qRear;
    uint16_t qSize; 
    uint8_t qBuf[QUEUE_BUF_LEN];
} RxLoopQueue; 


static RxLoopQueue g_RxQueue; 


void hs_RxLoopQueue_create(void)
{
    memset(&g_RxQueue, 0, sizeof(RxLoopQueue));
}

void hs_RxLoopQueue_clear(void)
{
    memset(&g_RxQueue, 0, sizeof(RxLoopQueue));
}

uint8_t hs_RxLoopQueue_is_empty(void)
{
    return (g_RxQueue.qFront == g_RxQueue.qRear) ? 1 : 0;
}

uint8_t hs_RxLoopQueue_is_full(void)
{
    return g_RxQueue.qFront == (g_RxQueue.qRear + 1) % QUEUE_BUF_LEN ? 1 : 0;
}

void hs_RxLoopQueue_enter(uint8_t data)
{
    if(!hs_RxLoopQueue_is_full()) {
        g_RxQueue.qBuf[g_RxQueue.qRear] = data;
        g_RxQueue.qSize++;
        g_RxQueue.qRear = (g_RxQueue.qRear + 1) % QUEUE_BUF_LEN;
    }
}

uint8_t hs_RxLoopQueue_delete(void)
{
    uint8_t data = 0xff;

    if(!hs_RxLoopQueue_is_empty()) {
        data = g_RxQueue.qBuf[g_RxQueue.qFront];
        g_RxQueue.qSize--;
        g_RxQueue.qFront = (g_RxQueue.qFront + 1) % QUEUE_BUF_LEN;
    }

    return data;
}

uint16_t hs_RxLoopQueue_get_size(void)
{
    return g_RxQueue.qSize;
}

uint16_t hs_RxLoopQueue_get_data(uint8_t **data)
{
    uint16_t ulDataLen;
    
    *data = g_RxQueue.qBuf;
    ulDataLen = g_RxQueue.qSize;

    return ulDataLen;
}

uint16_t hs_RxLoopQueue_read_data(uint8_t *data, uint16_t size)
{
    uint16_t i, len = 0;
    
    len = size < g_RxQueue.qSize ? size : g_RxQueue.qSize;
    
    for(i=0; i<len; i++) {
        *data++ = hs_RxLoopQueue_delete();
    }
    
    return len;
}

uint16_t hs_RxLoopQueue_read_all(uint8_t *data)
{
    uint16_t len = 0;
    
    while(!hs_RxLoopQueue_is_empty()) {
        len++;
        *data++ = hs_RxLoopQueue_delete();
    }
    
    return len;
}

void hs_RxLoopQueue_test(void)
{
    hs_usart_init();
    
    hs_RxLoopQueue_create();
    
    while(1) {
        hs_delay_ms(30);
        while(hs_RxLoopQueue_get_size()) {
            printf("%c", hs_RxLoopQueue_delete());
        }
    }
}
