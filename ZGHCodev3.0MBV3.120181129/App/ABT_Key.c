#include "stdlib.h"
#include "string.h"

#include "ABT_Key.h"

#define HEAT_KEY PTB7

void ABT_HeatKeyInit(void)
{
    gpio_init(HEAT_KEY,GPI,0); 
}

uint8 ABT_GetHeatKeyStatus(void)
{
    uint8 preStatus = gpio_get(HEAT_KEY);
    
    DELAY_MS(20);
    
    if(preStatus == gpio_get(HEAT_KEY))
	return preStatus;
    else
	return 1;
}

void ABT_LightKey(uint16 blinkTime)
{
    static uint16 lightCnt = 0;
    
    if(blinkTime == 65535)
	led(LEDHEAT,LED_ON);
    else
    {
	if(lightCnt < blinkTime)
	    led(LEDHEAT,LED_ON);
	else
	    led(LEDHEAT,LED_OFF);
    
	if(lightCnt > (blinkTime<<1))
	    lightCnt = 0;
	else
	    lightCnt++;
    }
}