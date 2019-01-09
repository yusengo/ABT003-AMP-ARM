/*
---------炬远智能科技（上海）有限公司---------
---------版权所有 侵权必究-------
---------ZGHcodev3.1-------
*/

/*
1. 本程序在ZGHv4板卡上运行，修改部分硬件信息，
重要信息重申：
	Z1. 	U4DIR:	PTB19
		485RX:	PTC14
		485TX:	PTC15
    	Z2.	PRX:	PTC16
		PTX:	PTC17
	Z3.	MCUCRTL:PTA24
	Z4.	S0:	PTB21
	Z5.	PTR:	ADC1-CH15(abort)
	Z6.	PTV:	ADC1-CH14(abort)
	Z7.	PLSCNT:	PTD6 FTM0_C
改动信息如下：
	G1.	IOCTRL1:	PTA27 -> PTE24
		IOCTRL2:	PTA28 -> PTE25
	G2.	LED0:	PTA11 -> PTC4
	G3.	+LED1:	PTB9
		+KeyTest:	PTB7
2. 将AD7705驱动加入
	G4.	+AD_\C\S:	PTE0	SPI1_PCS1	out
		+AD_DIN:		PTE1	SPI1_SOUT	out
		+AD_SCLK:	PTE2	SPI1_SCK		out
		+AD_DOUT:	PTE3	SPI1_SIN		in
		+AD_\R\S\T:	PTE4	--		out
		+AD_\D\R\D\Y	PTE5	--		in
*/

#include "common.h"
#include "include.h"
#include "MBK60Set.h"
#include "mb.h"

#define REFPLS	0
#define MEAPLS	1

#define DEBUG_EN	0
#define DEBUG_T 	10
#define DEBUG_RH	10

#define ADC_PTV		ADC1_SE14
#define ADC_PTR		ADC1_SE15

#define EXADC_EN	1

volatile uint32 cnvtime		= 0;

void FTM0_INPUT_IRQHandler(void);

#define SELPLS	PTB21

void SelPlsInit(void)
{
    gpio_init(SELPLS,GPO,0); 
}

void PlsChoose(unsigned char mr)
{
  if(mr == REFPLS)
  	gpio_set(SELPLS, 0);                  //S0 == 0 
  else
  	gpio_set(SELPLS, 1);                  //S0 == 1    
}

//SPI0	ADC
//66	CS 	PTA14
//67	SCK 	PTA15
//68	SOUT	PTA16
//69	SIN	PTA17

#define CS_L	gpio_set(PTA14, 0)
#define CS_H	gpio_set(PTA14, 1)

#define SCK_L	gpio_set(PTA15, 0)
#define SCK_H	gpio_set(PTA15, 1)

#define SO_L	gpio_set(PTA16, 0)
#define SO_H	gpio_set(PTA16, 1)

void ABT_SPI_init(void)
{
   gpio_init(PTA14, GPO,0); 
   gpio_init(PTA15, GPO,0); 
   gpio_init(PTA16, GPO,0);
   
   gpio_set(PTA14, 1);
   gpio_set(PTA15, 1);
   gpio_set(PTA16, 1);
}

void DA_convert(uint16 num)
{
    uint8 i = 0;
    uint16 numTemp = num;
    
    SCK_H;
    CS_L;
    
    for(i = 0;i < 16;i++)
    {
	if(numTemp&0x8000)
	    SO_H;
	else
	    SO_L;
	SCK_L;
	DELAY_US(1);
	numTemp <<= 1;
	SCK_H;
	DELAY_US(1);
    }
    SCK_H;
    CS_H;
}

void GlobalParaInit(void)
{
    GH_MB_GlobalData_t.G_MB_RESET_TRANSMITTER  	= 0;
    GH_MB_GlobalData_t.G_MB_RH			= 0.0;
    GH_MB_GlobalData_t.G_MB_SET_DEBUG_EN 	= 0;
    GH_MB_GlobalData_t.G_MB_T 			= 0.0;
    GH_MB_GlobalData_t.G_MB_SET_HEAT_SELF_TEST 	= 0;
}

void main()
{
    char comCmd 	= 0x00;
    char dataIndex	= 1;

    uint32 freqMea	= 0;
    uint32 freqRef	= 0;
    
    uint16 RHduty	= 0;
    uint16 Tduty	= 0;


    float tempValue	= 0.0;
    float rhadj	= 0.0;
    float rhldy	= 0.0;
    
    set_priority_grouping(NVIC_PriorityGroup_4);
    
    SelPlsInit();
    
    ABT_HeatKeyInit();
    led_init(LED0);
    led_init(LEDHEAT);
    led_init(LMCUCTRLHEAT);
    
    led(LED0,LED_OFF);
    led(LEDHEAT,LED_OFF);
    led(LMCUCTRLHEAT,LED_ON);
    
    ftm_input_init(FTM0,FTM_CH6,FTM_Falling,FTM_PS_2);
    
    printf("A");
    if(EXADC_EN == 1)
    	ABT_AD7705Init();
    else
    {
    	adc_init(ADC1_SE14);              //ADC PTVOLT
    	adc_init(ADC1_SE15);              //ADC PTREF
    }
    
    ABT_SPI_init();
    set_vector_handler(FTM0_VECTORn,FTM0_INPUT_IRQHandler);
    set_irq_priority(FTM0_IRQn,3);
    enable_irq(FTM0_IRQn);
    
    GlobalParaInit();
    ABT_FlashInit();
    
    if(ABT_LoadJudge() == TRUE)
    	ABT_ParaLoadFromFlash();
    else
    	ABT_ParaLoadFromFactory();
    
    DELAY_MS(1000);

    printf("A");
    ABT_MBInit();
    /*while(1)
    {
	eMBPoll();
    }

    while(1);*/
    while(1)
    {
	eMBPoll();
	/*
	if(0 == ABT_GetHeatKeyStatus())
	{
	    GH_MB_GlobalData_t.G_MB_SET_HEAT_SELF_TEST = 1;
	}
	*/
	if(GH_MB_GlobalData_t.G_MB_SET_HEAT_SELF_TEST)
	{
	    led (LEDHEAT,LED_ON);//加热
	    led (LMCUCTRLHEAT,LED_OFF);//加热
	}
	else
	{
	    led (LEDHEAT,LED_OFF);//不加热
	    led (LMCUCTRLHEAT,LED_ON);//不加热
	}
	
	if(EXADC_EN == 1)
	{
	    adcValue 	= KalmanFilterADC(ABT_AD7705_ReadAdc(1));
	    adcRefValue = KalmanFilterADCREF(ABT_AD7705_ReadAdc(2));       
	}
	else
	{
	    adcValue 	= KalmanFilterADC(adc_once(ADC_PTV, ADC_16bit));
	    adcRefValue = KalmanFilterADCREF(adc_once(ADC_PTR, ADC_16bit));
	}
	
	//adcValue = 15000;
	//adcRefValue = 9999;
	
	pTomValue = adcValue * 1000.0f / adcRefValue;
	    
	tempValue = TempNowCalculate(pTomValue);
	
	//GH_MB_GlobalData_t.G_MB_T = tempValue;
	GH_MB_GlobalData_t.G_MB_T = 21.99;
	
	Tduty = TDutyCalculate(tempValue); 

	GH_MB_GlobalData_t.G_MB_SET_DEBUG_EN = 12;
	//DACB
	if(GH_MB_GlobalData_t.G_MB_SET_DEBUG_EN)
	    DA_convert((0X4000|0X1000|(0x0fff&((GH_MB_FlashData_t.F_LIST_TIOUT_420_MA)[GH_MB_GlobalData_t.G_MB_SET_DEBUG_EN]))));
	else
	    DA_convert((0X4000|0X1000|(0x0fff&(Tduty))));//Tout
	
	if(cnvtime != 0)
	{    
	  PlsChoose(REFPLS);
	  DELAY_MS(20); 
	  cnvtime = 0;
	  FTM0_STATUS = 0x00;
	  FTM_IRQ_EN(FTM0,FTM_CH0);
	  
	  while(cnvtime == 0);
	  
	  refPlsCnt	= (uint32)(KalmanFilterREFPLS(cnvtime) + 0.5);
	  //refPlsCnt = 15999;
	  
	  freqRef = ((bus_clk_khz*1000)/((1<<FTM_PS_2)*refPlsCnt));
	  	  
	  PlsChoose(MEAPLS);
	  DELAY_MS(20);   
	  cnvtime = 0;
	  FTM0_STATUS = 0x00;
	  FTM_IRQ_EN(FTM0,FTM_CH0);
	  
	  while(cnvtime == 0);
	  
	  meaPlsCnt	= (uint32)(KalmanFilterMEAPLS(cnvtime) + 0.5);
	  //meaPlsCnt = 20000;
	  
	  freqMea = ((bus_clk_khz*1000)/((1<<FTM_PS_2)*meaPlsCnt));
	  
	  
	  meaTRatio =(uint16)((meaPlsCnt * 1.0/refPlsCnt)*10000.0 + 0.5);
	  
	  rhadj = RHadjNowCalculate(meaTRatio);
	  rhldy = RHldyNowCalculate(tempValue,rhadj);
	  
	  rhldy = KalmanFilterRH(rhldy);
	  
	  GH_MB_GlobalData_t.G_MB_RH = 30.599;//rhldy
	  //GH_MB_GlobalData_t.G_MB_RH = rhldy;
	  
	  RHduty = RHDutyCalculate(rhldy);
	  GH_MB_GlobalData_t.G_MB_SET_DEBUG_EN = 8;
	  if(GH_MB_GlobalData_t.G_MB_SET_DEBUG_EN)
	      DA_convert((0X4000|0X8000|(0x0fff&((GH_MB_FlashData_t.F_LIST_RHIOUT_420_MA)[GH_MB_GlobalData_t.G_MB_SET_DEBUG_EN]))));//RHout  
	  else
	      DA_convert((0X4000|0X8000|(0x0fff&(RHduty))));//RHout  
	  //DACA

	  uart_querychar(UART3, &comCmd);
	  //uart_putchar(UART3, comCmd);
	  if(comCmd == 'T')
	  {
	      printf("******   New Data Acquired with Num：%d   *****\n",dataIndex++);
	      printf("adcValue:   %d \n",(uint16)(adcValue + 0.5));
	      printf("adcREFValue:%d \n",(uint16)(adcRefValue + 0.5));
	      printf("pTomValue:  %d \n",(uint16)(pTomValue*10));
	      printf("meaPlsCnt:  %d \n",freqMea);
	      printf("refPlsCnt:  %d \n",freqRef);
	      printf("meaTRatio:  %d \n",meaTRatio);
	      printf("\n");
	  }
	  //20180406
	}
	else
	{}//printf("\n No freq detected! \n");
    }
}

void FTM0_INPUT_IRQHandler(void)
{
    static uint8 flag = 0;
    uint8 s = FTM0_STATUS;
    uint8 CHn;
    FTM0_STATUS = 0x00;
    
    CHn = 6;
    if(s &(1 <<CHn))
    {
	if(flag == 0)
	{
	    flag++;
	    ftm_input_clean(FTM0);
	}
	else
	    if(flag ==1)
	    {
		cnvtime = ftm_input_get(FTM0,FTM_CH6);
		flag = 0;
		FTM_IRQ_DIS(FTM0,FTM_CH0);
	    }
    }
}




