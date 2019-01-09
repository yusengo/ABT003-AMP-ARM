/*
---------炬远智能科技（上海）有限公司---------
---------版权所有 翻版必究-------------------
---------2018.07.18 yusengo@163.com---------
*/

/*
 * 包含头文件
 */
#include "common.h"
#include "MK60_port.h"
#include "MK60_gpio.h"
#include "VCAN_LED.H"


/*
 * 定义LED 编号对应的管脚
 */
PTXn_e LED_PTxn[LED_MAX] = {PTC4,PTB9,PTA24};


/*!
 *  @brief      初始化LED端口
 *  @param      LED_e    LED编号
 *  @since      v5.0
 *  Sample usage:       led_init (LED0);    //初始化 LED0
 */
void    led_init(LED_e ledn)
{
    if(ledn < LED_MAX)
    {
        gpio_init(LED_PTxn[ledn],GPO,LED_OFF);
    }
    else
    {
        ledn = LED_MAX;
        while(ledn--)
        {
            gpio_init(LED_PTxn[ledn],GPO,LED_OFF);
        }

    }
}



/*!
 *  @brief      设置LED灯亮灭
 *  @param      LED_e           LED编号（LED0、LED1、LED2、LED3）
 *  @param      LED_status      LED亮灭状态（LED_ON、LED_OFF）
 *  @since      v5.2
 *  Sample usage:       led (LED0,LED_ON);    //点亮 LED0
 */
void    led(LED_e ledn,LED_status status)
{
    if(ledn < LED_MAX)
    {
        gpio_set(LED_PTxn[ledn],status);
    }
    else
    {
        ledn = LED_MAX;
        while(ledn--)
        {
            gpio_set(LED_PTxn[ledn],status);
        }
    }
}

/*!
 *  @brief      设置LED灯亮灭反转
 *  @param      LED_e           LED编号（LED0、LED1、LED2、LED3）
 *  @since      v5.2
 *  Sample usage:       led_turn (LED0);    // LED0灯亮灭反转
 */
void led_turn(LED_e ledn)
{
    if(ledn < LED_MAX)
    {
        gpio_turn(LED_PTxn[ledn]);
    }
    else
    {
        ledn = LED_MAX;
        while(ledn--)
        {
            gpio_turn(LED_PTxn[ledn]);
        }
    }
}
