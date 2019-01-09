

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
void write_vtor (int);                                              //�����ж�������ĵ�ַ
void set_vector_handler(VECTORn_t , void pfunc_handler(void));      //�����жϺ������ж���������


//���ݾɴ���
#define set_priority_grouping(PGroup)	NVIC_SetPriorityGrouping(PGroup)//�����жϷ���
#define enable_irq(irq)                 NVIC_EnableIRQ(irq)         //ʹ��IRQ
#define disable_irq(irq)                NVIC_DisableIRQ(irq)        //��ֹIRQ
#define set_irq_priority(irq,pri0)      NVIC_SetPriority(irq,pri0)  //�������ȼ�

#if 1    //���ַ�����һ����CMSIS�Դ���һ����ֱ�ӻ������
#define EnableInterrupts                __enable_irq()              //ʹ��ȫ���ж�
#define DisableInterrupts               __disable_irq()             //��ֹȫ���ж�
#else
#define EnableInterrupts                asm(" CPSIE i");            //ʹ��ȫ���ж�
#define DisableInterrupts               asm(" CPSID i");            //��ֹȫ���ж�
#endif

void vcan_cpy( uint8 *dst, uint8 *src, uint32 count);

#endif  /* __MISC_H__ */

