

#ifndef __MISC_H__
#define __MISC_H__

#include "common.h"

#define NVIC_PriorityGroup_0          ((uint32)0x7) /* 0 bits for pre-emption priority
                                                      4 bits for subpriority */
#define NVIC_PriorityGroup_1          ((uint32)0x6) /* 1 bits for pre-emption priority
                                                      3 bits for subpriority */
#define NVIC_PriorityGroup_2          ((uint32)0x5) /* 2 bits for pre-emption priority
                                                      2 bits for subpriority */
#define NVIC_PriorityGroup_3          ((uint32)0x4) /* 3 bits for pre-emption priority
                                                      1 bits for subpriority */
#define NVIC_PriorityGroup_4          ((uint32)0x3) /* 4 bits for pre-emption priority
                                                      0 bits for subpriority */
void write_vtor (int);                                              //设置中断向量表的地址
void set_vector_handler(VECTORn_t , void pfunc_handler(void));      //设置中断函数到中断向量表里


//兼容旧代码
#define set_priority_grouping(PGroup)	NVIC_SetPriorityGrouping(PGroup)//设置中断分组
#define enable_irq(irq)                 NVIC_EnableIRQ(irq)         //使能IRQ
#define disable_irq(irq)                NVIC_DisableIRQ(irq)        //禁止IRQ
#define set_irq_priority(irq,pri0)      NVIC_SetPriority(irq,pri0)  //设置优先级

#if 1    //两种方法，一种是CMSIS自带，一种是直接汇编命令
#define EnableInterrupts                __enable_irq()              //使能全部中断
#define DisableInterrupts               __disable_irq()             //禁止全部中断
#else
#define EnableInterrupts                asm(" CPSIE i");            //使能全部中断
#define DisableInterrupts               asm(" CPSID i");            //禁止全部中断
#endif

void vcan_cpy( uint8 *dst, uint8 *src, uint32 count);

#endif  /* __MISC_H__ */

