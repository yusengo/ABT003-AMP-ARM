/*
---------炬远智能科技（上海）有限公司---------
---------版权所有 翻版必究-------------------
---------2018.07.18 yusengo@163.com---------
*/

#ifndef __MK60_CONF_H__
#define __MK60_CONF_H__

/*
 * 定义平台[放在工程选项里定义]
 */
//#define MK60DZ10
//#define MK60F15




/*
 * 选择是否输出调试信息，不输出则注释下面的宏定义
 */
#define DEBUG_PRINT

/*
 * 定义晶振时钟，单位为MHz
 */
#define EXTAL_IN_MHz            (50)


#if defined(MK60DZ10)			//以下为 MK60DZ10 的超频配置
/*
 * 定义 PLL 超频 频率
 */
#define CORE_CLK                PLL96       // 从 PLL_e 里选择 已提供的 配置方案
                                            // bus 、 flex bus 、 flash 频率 都是 core 整数倍分频而来
#define MAX_BUS_CLK             48          // bus      (bus        >= core/16  )
#define MAX_FLEXBUS_CLK         25          // flex bus (flex bus   >= core/16  )
#define MAX_FLASH_CLK           25          // flash    (flash      >= core/16  )

#elif defined(MK60F15)			//以下为 MK60F15 的超频配置

#define CORE_CLK                PLL120      // 从 PLL_e 里选择 已提供的 配置方案
                                            // bus 、 flex bus 、 flash 频率 都是 core 整数倍分频而来
#define MAX_BUS_CLK             60          // bus      (bus        >= core/16  )
#define MAX_FLEXBUS_CLK         25          // flex bus (flex bus   >= core/16  )
#define MAX_FLASH_CLK           25          // flash    (flash      >= core/16  )

#endif

/*********************   自定义 时钟频率 分频因子   ********************/
//如果 CORE_CLK 为 PLLUSR ，则为自定义模式 ，如下配置生效
//如果是 F15 系列： MCG_CLK_MHZ = 50u*(VDIV+16)/(PRDIV+1)/2
//如果是 DZ10 系列：MCG_CLK_MHZ = 50u/*(VDIV+24)(PRDIV+1)
#define PRDIV             10
#define VDIV              29
#define CORE_DIV          0         //  core = mcg/ ( CORE_DIV  + 1 )
#define BUS_DIV           1         //  bus  = mcg/ ( BUS_DIV   + 1 )
#define FLEX_DIV          9         //  flex = mcg/ ( FLEX_DIV  + 1 )
#define FLASH_DIV         8         //  flash= mcg/ ( FLASH_DIV + 1 )

/*
 * 定义 printf函数 的 串口输出端口 和 串口信息
 */
#define VCAN_PORT           UART3
#define VCAN_BAUD           115200

/*
 * 配置延时函数
 */
#if 0
#include "MK60_DWT.h"                       //有可能不稳定
#define DELAY()         dwt_delay_ms(500)
#define DELAY_MS(ms)    dwt_delay_ms(ms)
#define DELAY_US(us)    dwt_delay_us(us)
#elif   0
#include "MK60_lptmr.h"
#define     DELAY()         lptmr_delay_ms(500)
#define     DELAY_MS(ms)    lptmr_delay_ms(ms)
#define     DELAY_US(us)    lptmr_delay_us(us)
#elif   0
#include "MK60_pit.h"
#define DELAY()         pit_delay_ms(PIT3,500)
#define DELAY_MS(ms)    pit_delay_ms(PIT3,ms)
#define DELAY_US(us)    pit_delay_us(PIT3,us)
#else
#include "MK60_SysTick.h"
#define DELAY()         systick_delay_ms(500)
#define DELAY_MS(ms)    systick_delay_ms(ms)
#define DELAY_US(us)    systick_delay_us(us)
#endif


/*
 * 配置断言和其实现函数
 */
void assert_failed(char *, int);

#if defined( DEBUG )
#define ASSERT(expr) \
    if (!(expr)) \
        assert_failed(__FILE__, __LINE__)
#else
#define ASSERT(expr)
#endif

/*
 * 配置调试输出函数
 */
#if( defined(DEBUG) && defined(DEBUG_PRINT))
#define DEBUG_PRINTF(FORMAT,...)        do{printf(FORMAT,##__VA_ARGS__);}while(0)	/*无需打印调试信息时，请将宏内容注释掉*/
#else
#define DEBUG_PRINTF(FORMAT,...)
#endif

/*
 * 启动前检测，防止 复用下载口
 */
void start_check();
#ifdef DEBUG
#define SRART_CHECK()       start_check()
#else
#define SRART_CHECK()
#endif


#endif /* __MK60_CONF_H__  */
