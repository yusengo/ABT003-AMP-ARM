/*
---------炬远智能科技（上海）有限公司----
---------ZGHcodev3.0---------------------
-------ModBusSet for K60--------------------
*/

#include "MBK60Set.h"
#include "mb.h"
#include "mbport.h"
#include  "MK60_PIT.h"    

extern char rxDataFirst = 0;
extern char rxData = 0;

static char ch = 0;

void ABT_MBUartRXIRQEN(void)
{    
    gpio_set(MB_DIR_PORT, MB_DIR_RX);

    uart_rx_irq_en(MB_RXTX_PORT);//recv intr
}

void ABT_MBUartRXIRQDIS(void)
{
    gpio_set(MB_DIR_PORT, MB_DIR_TX);

    uart_rx_irq_dis(MB_RXTX_PORT);//禁止recv intr
}

void ABT_MBUartTXIRQEN(void)
{
    gpio_set(MB_DIR_PORT, MB_DIR_TX);

    uart_tx_irq_en(MB_RXTX_PORT);//tx intr
}

void ABT_MBUartTXIRQDIS(void)
{
    gpio_set(MB_DIR_PORT, MB_DIR_RX);

    uart_tx_irq_dis(MB_RXTX_PORT);//禁止tx intr
}

/***************************************************************************/
void ABT_MBPitExpired_handler(void)
{   
    prvvTIMERExpiredISR();//-> pxMBPortCBTimerExpired(); == xMBRTUTimerT35Expired();
    led_turn(LED0);

    PIT_Flag_Clear(PIT0);
}

void ABT_MBTimer35TInit(uint32 timeOut)
{
    //pit_init_ms(PIT0,timeOut);
    pit_init_ms(PIT0,2);
    
    set_vector_handler(PIT0_VECTORn,ABT_MBPitExpired_handler);    
    set_irq_priority(PIT0_IRQn,1);
    
    enable_irq(PIT0_IRQn);
    printf("timer is ok\n");
}

void ABT_MBTimerEnable(void)
{	
    PIT_Flag_Clear(PIT0);                           //清中断标志位

    PIT_TCTRL(PIT0) &= ~ PIT_TCTRL_TEN_MASK;        //禁止PITn定时器（用于清空计数值）
    PIT_TCTRL(PIT0)  = ( 0
                         | PIT_TCTRL_TEN_MASK        //使能 PITn定时器
                         | PIT_TCTRL_TIE_MASK        //开PITn中断
                       );
    
    enable_irq(PIT0_IRQn);  
}

void ABT_MBTimerDisable(void)
{
    PIT_Flag_Clear(PIT0);                           //清中断标志位

    PIT_TCTRL(PIT0) &= ~ PIT_TCTRL_TEN_MASK;        //禁止PITn定时器（用于清空计数值）
    PIT_TCTRL(PIT0)  = ( 0
                         | PIT_TCTRL_TEN_MASK        //使能 PITn定时器
                         | PIT_TCTRL_TIE_MASK        //开PITn中断
                       );
    
    disable_irq(PIT0_IRQn);    
}

/***************************************************************************/
void ABT_MBUartPutChar(char ch)
{
    uart_putchar(MB_RXTX_PORT,ch);     
}

void ABT_MBUartGetChar(char *ch)
{
    uart_getchar(MB_RXTX_PORT,ch);
}

  
void ABT_MBUart_handler(void)
{
   //static char ch = 0;
   if(UART_S1_REG(UARTN[MB_RXTX_PORT]) & UART_S1_RDRF_MASK)//接收中断
   {
       /*ABT_MBUartGetChar(&ch);
       if(ch == 'T')
	   ABT_MBTimerEnable();*/
	ABT_MBUartGetChar(&rxData);
    
	/*if(rxDataFirst == 0)
	    rxDataFirst = rxData;*/
	/*if(ch == 0)
	    ch = rxData;
	*/
       prvvUARTRxISR();//prvvUARTRxISR()   -> pxMBFrameCBByteReceived(  ) == xMBRTUReceiveFSM
       UART_S1_REG(UARTN[MB_RXTX_PORT]) |= UART_S1_RDRF_MASK;

   }
   else
     if(UART_S1_REG(UARTN[MB_RXTX_PORT]) & UART_S1_TDRE_MASK)//发送中断
     {
	 prvvUARTTxReadyISR();//prvvUARTTxReadyISR() ->   pxMBFrameCBTransmitterEmpty() == xMBRTUTransmitFSM

	 UART_S1_REG(UARTN[MB_RXTX_PORT]) |= UART_S1_TDRE_MASK;

     }     
}

void ABT_MBUartInit(uint32 baud)
{
    MBuart_init(baud);//odd 115200
    
    gpio_init(MB_DIR_PORT, GPO, MB_DIR_RX);
    gpio_set(MB_DIR_PORT, MB_DIR_RX); 
    
    set_vector_handler(UART4_RX_TX_VECTORn,ABT_MBUart_handler);
    
    uart_rx_irq_en(MB_RXTX_PORT);//recv intr
    
    set_irq_priority(UART4_RX_TX_IRQn,0);//highest 
    printf("serial is ok\n");
}
  
void ABT_MBInit(void)
{
	//eMBErrorCode    eStatus;

	eMBInit(MB_RTU, 35, 0, 115200, MB_PAR_ODD);

	/* Enable the Modbus Protocol Stack. */
	eMBEnable();	
}
