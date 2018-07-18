#include "main.h"


#define DOOR_RELAY             PA12
#define DOOR_RELAY_PORT        PA
#define DOOR_RELAY_PIN         BIT12

#define DOOR_STATE             PB0
#define DOOR_STATE_PORT        PB
#define DOOR_STATE_PIN         BIT0

#define DOOR_BUTTON            PD15
#define DOOR_BUTTON_PORT       PD
#define DOOR_BUTTON_PIN        BIT15

#define WEIGEN_D0              PC14
#define WEIGEN_D0_PORT         PC
#define WEIGEN_D0_PIN          BIT14

#define WEIGEN_D1              PD14
#define WEIGEN_D1_PORT         PD
#define WEIGEN_D1_PIN          BIT14


static void hs_weigen_init(void)
{
    GPIO_SetMode(WEIGEN_D0_PORT, WEIGEN_D0_PIN, GPIO_PMD_OUTPUT);
    GPIO_SetMode(WEIGEN_D1_PORT, WEIGEN_D1_PIN, GPIO_PMD_OUTPUT);
    
    WEIGEN_D0 = 0; WEIGEN_D1 = 0;
}

static void hs_relay_init(void)
{
    GPIO_SetMode(DOOR_RELAY_PORT, DOOR_RELAY_PIN, GPIO_PMD_OUTPUT);
    DOOR_RELAY = 0;
}

static void hs_relay_open(void)
{
    DOOR_RELAY = 1;
}

static void hs_relay_close(void)
{
    DOOR_RELAY = 0;
}

static void hs_weigen_open(void)
{

}

static void hs_weigen_close(void)
{
    
}

void hs_door_init(void)
{
    hs_weigen_init();
    hs_relay_init();
    GPIO_SetMode(DOOR_STATE_PORT, DOOR_STATE_PIN, GPIO_PMD_INPUT);
    GPIO_SetMode(DOOR_BUTTON_PORT, DOOR_BUTTON_PIN, GPIO_PMD_INPUT);
    GPIO_ENABLE_PULL_UP(DOOR_STATE_PORT, DOOR_STATE_PIN);
    GPIO_ENABLE_PULL_UP(DOOR_BUTTON_PORT, DOOR_BUTTON_PIN);
}

void hs_door_open(uint8_t method)
{
    if(HS_CFG_OPEN_METHOD_RELAY == method) {
        hs_relay_open();
    } else if(HS_CFG_OPEN_METHOD_WEIGEN == method) {
        hs_weigen_open();
    }
    g_stFlag.ucCtrDoorFlag = 1;
}

void hs_door_close(uint8_t method)
{
    if(HS_CFG_OPEN_METHOD_RELAY == method) {
        hs_relay_close();
    } else if(HS_CFG_OPEN_METHOD_WEIGEN == method) {
        hs_weigen_close();
    }
    g_stFlag.ucCtrDoorFlag = 2;
}

uint8_t hs_door_read_state(void)
{
    return DOOR_STATE;
}

uint8_t hs_door_read_button(void)
{
    return DOOR_BUTTON;
}

