#include "stdlib.h"
#include "string.h"

#include "AD7705.h"

/* ͨ��1��ͨ��2������,���뻺�壬���� */
#define __CH1_GAIN_BIPOLAR_BUF	(GAIN_1 | UNIPOLAR | BUF_EN)
#define __CH2_GAIN_BIPOLAR_BUF	(GAIN_1 | UNIPOLAR | BUF_EN)

/*
		+AD_\C\S:	PTE0	SPI1_PCS1	out
		+AD_DIN:	PTE1	SPI1_SOUT	out
		+AD_SCLK:	PTE2	SPI1_SCK	out
		+AD_DOUT:	PTE3	SPI1_SIN	in
		+AD_\R\S\T:	PTE4	--		out
		+AD_\D\R\D\Y	PTE5	--		in
*/

/* ����GPIO�˿� */
#define MCUPIN_CS	PTE0
#define MCUPIN_DOUT	PTE1
#define MCUPIN_SCK	PTE2
#define MCUPIN_RESET	PTE4

#define MCUPIN_DIN	PTE3
#define MCUPIN_DRDY	PTE5

/* ���������0����1�ĺ� */
#define RESET_0()	gpio_set(MCUPIN_RESET, 0)
#define RESET_1()	gpio_set(MCUPIN_RESET, 1)

#define CS_0()		gpio_set(MCUPIN_CS, 0)
#define CS_1()		gpio_set(MCUPIN_CS, 1)

#define SCK_0()		gpio_set(MCUPIN_SCK, 0)
#define SCK_1()		gpio_set(MCUPIN_SCK, 1)

#define DI_0()		gpio_set(MCUPIN_DOUT, 0)
#define DI_1()		gpio_set(MCUPIN_DOUT, 1)

#define DO_IS_HIGH()	(gpio_get(MCUPIN_DIN)  == 1)

#define DRDY_IS_LOW()	(gpio_get(MCUPIN_DRDY) == 0)

/* ͨ�żĴ���bit���� */
enum
{
    /* �Ĵ���ѡ��  RS2 RS1 RS0  */
    REG_COMM	= 0x00,	/* ͨ�żĴ��� */
    REG_SETUP	= 0x10,	/* ���üĴ��� */
    REG_CLOCK	= 0x20,	/* ʱ�ӼĴ��� */
    REG_DATA	= 0x30,	/* ���ݼĴ��� */
    REG_ZERO_CH1	= 0x60,	/* CH1 ƫ�ƼĴ��� */
    REG_FULL_CH1	= 0x70,	/* CH1 �����̼Ĵ��� */
    REG_ZERO_CH2	= 0x61,	/* CH2 ƫ�ƼĴ��� */
    REG_FULL_CH2	= 0x71,	/* CH2 �����̼Ĵ��� */

    /* ��д���� */
    WRITE 		= 0x00,	/* д���� */
    READ 		= 0x08,	/* ������ */

    /* ͨ�� */
    CH_1		= 0,	/* AIN1+  AIN1- */
    CH_2		= 1,	/* AIN2+  AIN2- */
    CH_3		= 2,	/* AIN1-  AIN1- */
    CH_4		= 3	/* AIN1-  AIN2- */
};

/* ���üĴ���bit���� */
enum
{
    MD_NORMAL		= (0 << 6),	/* ����ģʽ */
    MD_CAL_SELF		= (1 << 6),	/* ��У׼ģʽ */
    MD_CAL_ZERO		= (2 << 6),	/* У׼0�̶�ģʽ */
    MD_CAL_FULL		= (3 << 6),	/* У׼���̶�ģʽ */

    GAIN_1			= (0 << 3),	/* ���� */
    GAIN_2			= (1 << 3),	/* ���� */
    GAIN_4			= (2 << 3),	/* ���� */
    GAIN_8			= (3 << 3),	/* ���� */
    GAIN_16			= (4 << 3),	/* ���� */
    GAIN_32			= (5 << 3),	/* ���� */
    GAIN_64			= (6 << 3),	/* ���� */
    GAIN_128		= (7 << 3),	/* ���� */

    /* ����˫���Ի��ǵ����Զ����ı��κ������źŵ�״̬����ֻ�ı�������ݵĴ����ת�������ϵ�У׼�� */
    BIPOLAR			= (0 << 2),	/* ˫�������� */
    UNIPOLAR		= (1 << 2),	/* ���������� */

    BUF_NO			= (0 << 1),	/* �����޻��壨�ڲ�������������) */
    BUF_EN			= (1 << 1),	/* �����л��� (�����ڲ�������) */

    FSYNC_0			= 0,
    FSYNC_1			= 1		/* ������ */
};

/* ʱ�ӼĴ���bit���� */
enum
{
    CLKDIS_0	= 0x00,		/* ʱ�����ʹ�� ������Ӿ���ʱ������ʹ�ܲ����񵴣� */
    CLKDIS_1	= 0x10,		/* ʱ�ӽ�ֹ �����ⲿ�ṩʱ��ʱ�����ø�λ���Խ�ֹMCK_OUT�������ʱ����ʡ�� */

    /*
	    2.4576MHz��CLKDIV=0 ����Ϊ 4.9152MHz ��CLKDIV=1 ����CLK Ӧ�� ��0����
	    1MHz ��CLKDIV=0 ���� 2MHz   ��CLKDIV=1 ����CLK ��λӦ��  ��1��
    */
    CLK_4_9152M = 0x08,
    CLK_2_4576M = 0x00,
    CLK_1M 	= 0x04,
    CLK_2M 	= 0x0C,

    FS_50HZ	= 0x00,
    FS_60HZ	= 0x01,
    FS_250HZ	= 0x02,
    FS_500HZ	= 0x04,
    /*
		    ��ʹ����ʱ��Ϊ 2.4576MHz ʱ��ǿ�ҽ��齫ʱ�ӼĴ�����Ϊ 84H,��ʱ�������������Ϊ10Hz,��ÿ0.1S ���һ�������ݡ�
		    ��ʹ����ʱ��Ϊ 1MHz ʱ��ǿ�ҽ��齫ʱ�ӼĴ�����Ϊ80H, ��ʱ�������������Ϊ4Hz, ��ÿ0.25S ���һ��������
    */
    ZERO_0		= 0x00,
    ZERO_1		= 0x80
};


static void ABT_AD7705_SyncSPI(void);
static void ABT_AD7705_Send8Bit(uint8_t _data);
static uint8_t ABT_AD7705_Recive8Bit(void);
static void ABT_AD7705_WriteByte(uint8_t _data);
static void ABT_AD7705_Write3Byte(uint32_t _data);
static uint8_t ABT_AD7705_ReadByte(void);
static uint16_t ABT_AD7705_Read2Byte(void);
static uint32_t ABT_AD7705_Read3Byte(void);
static void ABT_AD7705_WaitDRDY(void);
static void ABT_AD7705_ResetHard(void);
static void ABT_AD7705_Delay(void);
static void ABT_AD7705_CalibSelf(uint8_t _ch);
/*
*********************************************************************************************************
*	�� �� ��: ABT_AD7705Init
*	����˵��: ����STM32��GPIO��SPI�ӿڣ��������� ABT_AD7705
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ABT_AD7705Init(void)
{   
/*
#define MCUPIN_CS	PTE0
#define MCUPIN_DOUT	PTE1
#define MCUPIN_SCK	PTE2
#define MCUPIN_RESET	PTE4

#define MCUPIN_DIN	PTE3
#define MCUPIN_DRDY	PTE5
    */
    //gpio_init (PTXn_e ptxn, GPIO_CFG cfg, uint8 data)
    gpio_init (MCUPIN_CS,   GPO, 1);//PIN_CS output ��if in ��last para is omitted
    gpio_init (MCUPIN_DOUT, GPO, 0);//PIN_CS output ��if in ��last para is omitted
    gpio_init (MCUPIN_SCK,  GPO, 0);//PIN_CS output ��if in ��last para is omitted
    gpio_init (MCUPIN_RESET,GPO, 1);//PIN_CS output ��if in ��last para is omitted
	
    gpio_init (MCUPIN_DIN, GPI, 0);//PIN_CS output ��if in ��last para is omitted
    gpio_init (MCUPIN_DRDY,GPI, 0);//PIN_CS output ��if in ��last para is omitted
	
    DELAY_MS(10);

    ABT_AD7705_ResetHard();		/* Ӳ����λ */

    /*
      �ڽӿ����ж�ʧ������£������DIN �ߵ�ƽ��д�����������㹻����ʱ�䣨���� 32������ʱ�����ڣ���
      ABT_AD7705 ����ص�Ĭ��״̬��
    */	
    DELAY_MS(5);

    ABT_AD7705_SyncSPI();		/* ͬ��SPI�ӿ�ʱ�� */

    DELAY_MS(5);

    /* ����ʱ�ӼĴ��� */
    ABT_AD7705_WriteByte(REG_CLOCK | WRITE | CH_1);			/* ��дͨ�żĴ�������һ����дʱ�ӼĴ��� */

    //ABT_AD7705_WriteByte(CLKDIS_0 | CLK_4_9152M | FS_50HZ);	/* ˢ������50Hz */
    ABT_AD7705_WriteByte(CLKDIS_0 | CLK_4_9152M | FS_500HZ);	/* ˢ������500Hz */

    /* ÿ���ϵ����һ����У׼ */
    ABT_AD7705_CalibSelf(1);	/* �ڲ���У׼ CH1 */
    DELAY_MS(5);
    
    ABT_AD7705_CalibSelf(1);			
    ABT_AD7705_ReadAdc(1);				
	    
    ABT_AD7705_CalibSelf(2);		
    ABT_AD7705_ReadAdc(2);	
}

/*
*********************************************************************************************************
*	�� �� ��: ABT_AD7705_Delay
*	����˵��: CLK֮����ӳ٣�ʱ���ӳ�. ����STM32F407  168M��Ƶ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void ABT_AD7705_Delay(void)
{
    uint16_t i;

    for (i = 0; i < 5; i++);
}

/*
*********************************************************************************************************
*	�� �� ��: ABT_AD7705_ResetHard
*	����˵��: Ӳ����λ ABT_AD7705оƬ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void ABT_AD7705_ResetHard(void)
{
    RESET_1();
    DELAY_MS(1);	
    RESET_0();
    DELAY_MS(2);
    RESET_1();
    DELAY_MS(1);
}

/*
*********************************************************************************************************
*	�� �� ��: ABT_AD7705_SyncSPI
*	����˵��: ͬ��ABT_AD7705оƬSPI�ӿ�ʱ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void ABT_AD7705_SyncSPI(void)
{
    /* AD7705���нӿ�ʧ�����临λ����λ��Ҫ��ʱ500us�ٷ��� */
    CS_0();
    ABT_AD7705_Send8Bit(0xFF);
    ABT_AD7705_Send8Bit(0xFF);
    ABT_AD7705_Send8Bit(0xFF);
    ABT_AD7705_Send8Bit(0xFF);	
    CS_1();
}

/*
*********************************************************************************************************
*	�� �� ��: ABT_AD7705_Send8Bit
*	����˵��: ��SPI���߷���8��bit���ݡ� ����CS���ơ�
*	��    ��: _data : ����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void ABT_AD7705_Send8Bit(uint8_t _data)
{
    uint8_t i;

    for(i = 0; i < 8; i++)
    {
	    if (_data & 0x80)
	    {
		    DI_1();
	    }
	    else
	    {
		    DI_0();
	    }
	    SCK_0();
	    _data <<= 1;
	    ABT_AD7705_Delay();
	    SCK_1();	
	    ABT_AD7705_Delay();		
    }
}

/*
*********************************************************************************************************
*	�� �� ��: ABT_AD7705_Recive8Bit
*	����˵��: ��SPI���߽���8��bit���ݡ� ����CS���ơ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static uint8_t ABT_AD7705_Recive8Bit(void)
{
    uint8_t i;
    uint8_t read = 0;

    for (i = 0; i < 8; i++)
    {
	    SCK_0();
	    ABT_AD7705_Delay();		
	    read = read<<1;
	    if (DO_IS_HIGH())
	    {
		    read++;
	    }
	    SCK_1();		
	    ABT_AD7705_Delay();
    }
    return read;
}

/*
*********************************************************************************************************
*	�� �� ��: ABT_AD7705_WriteByte
*	����˵��: д��1���ֽڡ���CS����
*	��    ��: _data ����Ҫд�������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void ABT_AD7705_WriteByte(uint8_t _data)
{
    CS_0();
    ABT_AD7705_Send8Bit(_data);
    CS_1();
}

/*
*********************************************************************************************************
*	�� �� ��: ABT_AD7705_Write3Byte
*	����˵��: д��3���ֽڡ���CS����
*	��    ��: _data ����Ҫд�������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void ABT_AD7705_Write3Byte(uint32_t _data)
{
    CS_0();
    ABT_AD7705_Send8Bit((_data >> 16) & 0xFF);
    ABT_AD7705_Send8Bit((_data >> 8) & 0xFF);
    ABT_AD7705_Send8Bit(_data);
    CS_1();
}

/*
*********************************************************************************************************
*	�� �� ��: ABT_AD7705_ReadByte
*	����˵��: ��ADоƬ��ȡһ���֣�16λ��
*	��    ��: ��
*	�� �� ֵ: ��ȡ���֣�16λ��
*********************************************************************************************************
*/
static uint8_t ABT_AD7705_ReadByte(void)
{
    uint8_t read;

    CS_0();
    read = ABT_AD7705_Recive8Bit();
    CS_1();

    return read;
}

/*
*********************************************************************************************************
*	�� �� ��: ABT_AD7705_Read2Byte
*	����˵��: ��2�ֽ�����
*	��    ��: ��
*	�� �� ֵ: ��ȡ�����ݣ�16λ��
*********************************************************************************************************
*/
static uint16_t ABT_AD7705_Read2Byte(void)
{
    uint16_t read;

    CS_0();
    read = ABT_AD7705_Recive8Bit();
    read <<= 8;
    read += ABT_AD7705_Recive8Bit();
    CS_1();

    return read;
}

/*
*********************************************************************************************************
*	�� �� ��: ABT_AD7705_Read3Byte
*	����˵��: ��3�ֽ�����
*	��    ��: ��
*	�� �� ֵ: ��ȡ�������ݣ�24bit) ��8λ�̶�Ϊ0.
*********************************************************************************************************
*/
static uint32_t ABT_AD7705_Read3Byte(void)
{
    uint32_t read;

    CS_0();
    read = ABT_AD7705_Recive8Bit();
    read <<= 8;
    read += ABT_AD7705_Recive8Bit();
    read <<= 8;
    read += ABT_AD7705_Recive8Bit();
    CS_1();
    return read;
}

/*
*********************************************************************************************************
*	�� �� ��: ABT_AD7705_WaitDRDY
*	����˵��: �ȴ��ڲ�������ɡ� ��У׼ʱ��ϳ�����Ҫ�ȴ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void ABT_AD7705_WaitDRDY(void)
{
    uint32_t i;

    for (i = 0; i < 4000000; i++)
    {
	    if (DRDY_IS_LOW())
	    {
		    break;
	    }
    }
    if (i >= 4000000)
    {

    }
}

/*
*********************************************************************************************************
*	�� �� ��: ABT_AD7705_WriteReg
*	����˵��: дָ���ļĴ���
*	��    ��:  _RegID : �Ĵ���ID
*			  _RegValue : �Ĵ���ֵ�� ����8λ�ļĴ�����ȡ32λ�βεĵ�8bit
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ABT_AD7705_WriteReg(uint8_t _RegID, uint32_t _RegValue)
{
    uint8_t bits;

    switch (_RegID)
    {
	    case REG_COMM:		/* ͨ�żĴ��� */		
	    case REG_SETUP:		/* ���üĴ��� 8bit */
	    case REG_CLOCK:		/* ʱ�ӼĴ��� 8bit */
		    bits = 8;
		    break;

	    case REG_ZERO_CH1:	/* CH1 ƫ�ƼĴ��� 24bit */
	    case REG_FULL_CH1:	/* CH1 �����̼Ĵ��� 24bit */
	    case REG_ZERO_CH2:	/* CH2 ƫ�ƼĴ��� 24bit */
	    case REG_FULL_CH2:	/* CH2 �����̼Ĵ��� 24bit*/
		    bits = 24;
		    break;

	    case REG_DATA:		/* ���ݼĴ��� 16bit */
	    default:
		    return;
    }

    ABT_AD7705_WriteByte(_RegID | WRITE);	/* дͨ�żĴ���, ָ����һ����д��������ָ��д�ĸ��Ĵ��� */

    if (bits == 8)
    {
	    ABT_AD7705_WriteByte((uint8_t)_RegValue);
    }
    else	/* 24bit */
    {
	    ABT_AD7705_Write3Byte(_RegValue);
    }
}

/*
*********************************************************************************************************
*	�� �� ��: ABT_AD7705_ReadReg
*	����˵��: дָ���ļĴ���
*	��    ��:  _RegID : �Ĵ���ID
*			  _RegValue : �Ĵ���ֵ�� ����8λ�ļĴ�����ȡ32λ�βεĵ�8bit
*	�� �� ֵ: �����ļĴ���ֵ�� ����8λ�ļĴ�����ȡ32λ�βεĵ�8bit
*********************************************************************************************************
*/
uint32_t ABT_AD7705_ReadReg(uint8_t _RegID)
{
    uint8_t bits;
    uint32_t read;

    switch (_RegID)
    {
	    case REG_COMM:		/* ͨ�żĴ��� */
	    case REG_SETUP:		/* ���üĴ��� 8bit */
	    case REG_CLOCK:		/* ʱ�ӼĴ��� 8bit */
		    bits = 8;
		    break;

	    case REG_ZERO_CH1:	/* CH1 ƫ�ƼĴ��� 24bit */
	    case REG_FULL_CH1:	/* CH1 �����̼Ĵ��� 24bit */
	    case REG_ZERO_CH2:	/* CH2 ƫ�ƼĴ��� 24bit */
	    case REG_FULL_CH2:	/* CH2 �����̼Ĵ��� 24bit*/
		    bits = 24;
		    break;

	    case REG_DATA:		/* ���ݼĴ��� 16bit */
	    default:
		    return 0xFFFFFFFF;
    }

    ABT_AD7705_WriteByte(_RegID | READ);	/* дͨ�żĴ���, ָ����һ����д��������ָ��д�ĸ��Ĵ��� */

    if (bits == 16)
    {
	    read = ABT_AD7705_Read2Byte();
    }
    else if (bits == 8)
    {
	    read = ABT_AD7705_ReadByte();
    }
    else	/* 24bit */
    {
	    read = ABT_AD7705_Read3Byte();
    }
    return read;
}

/*
*********************************************************************************************************
*	�� �� ��: ABT_AD7705_CalibSelf
*	����˵��: ������У׼. �ڲ��Զ��̽�AIN+ AIN-У׼0λ���ڲ��̽ӵ�Vref У׼��λ���˺���ִ�й��̽ϳ���
*			  ʵ��Լ 180ms
*	��    ��:  _ch : ADCͨ����1��2
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ABT_AD7705_CalibSelf(uint8_t _ch)
{
      if (_ch == 1)
      {
	      /* ��У׼CH1 */
	      ABT_AD7705_WriteByte(REG_SETUP | WRITE | CH_1);	/* дͨ�żĴ�������һ����д���üĴ�����ͨ��1 */		
	      ABT_AD7705_WriteByte(MD_CAL_SELF | __CH1_GAIN_BIPOLAR_BUF | FSYNC_0);/* ������У׼ */
	      ABT_AD7705_WaitDRDY();	/* �ȴ��ڲ�������� --- ʱ��ϳ���Լ180ms */
      }
      else if (_ch == 2)
      {
	      /* ��У׼CH2 */
	      ABT_AD7705_WriteByte(REG_SETUP | WRITE | CH_2);	/* дͨ�żĴ�������һ����д���üĴ�����ͨ��2 */
	      ABT_AD7705_WriteByte(MD_CAL_SELF | __CH2_GAIN_BIPOLAR_BUF | FSYNC_0);	/* ������У׼ */
	      ABT_AD7705_WaitDRDY();	/* �ȴ��ڲ��������  --- ʱ��ϳ���Լ180ms */
      }
}

/*
*********************************************************************************************************
*	�� �� ��: ABT_AD7705_SytemCalibZero
*	����˵��: ����ϵͳУ׼��λ. �뽫AIN+ AIN-�̽Ӻ�ִ�иú�����У׼Ӧ������������Ʋ�����У׼������
*			 ִ����Ϻ󡣿���ͨ�� ABT_AD7705_ReadReg(REG_ZERO_CH1) ��  ABT_AD7705_ReadReg(REG_ZERO_CH2) ��ȡУ׼������
*	��    ��: _ch : ADCͨ����1��2
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ABT_AD7705_SytemCalibZero(uint8_t _ch)
{
      if (_ch == 1)
      {
	      /* У׼CH1 */
	      ABT_AD7705_WriteByte(REG_SETUP | WRITE | CH_1);	/* дͨ�żĴ�������һ����д���üĴ�����ͨ��1 */
	      ABT_AD7705_WriteByte(MD_CAL_ZERO | __CH1_GAIN_BIPOLAR_BUF | FSYNC_0);/* ������У׼ */
	      ABT_AD7705_WaitDRDY();	/* �ȴ��ڲ�������� */
      }
      else if (_ch == 2)
      {
	      /* У׼CH2 */
	      ABT_AD7705_WriteByte(REG_SETUP | WRITE | CH_2);	/* дͨ�żĴ�������һ����д���üĴ�����ͨ��1 */
	      ABT_AD7705_WriteByte(MD_CAL_ZERO | __CH2_GAIN_BIPOLAR_BUF | FSYNC_0);	/* ������У׼ */
	      ABT_AD7705_WaitDRDY();	/* �ȴ��ڲ�������� */
      }
}

/*
*********************************************************************************************************
*	�� �� ��: ABT_AD7705_SytemCalibFull
*	����˵��: ����ϵͳУ׼��λ. �뽫AIN+ AIN-����������ѹԴ��ִ�иú�����У׼Ӧ������������Ʋ�����У׼������
*			 ִ����Ϻ󡣿���ͨ�� ABT_AD7705_ReadReg(REG_FULL_CH1) ��  ABT_AD7705_ReadReg(REG_FULL_CH2) ��ȡУ׼������
*	��    ��:  _ch : ADCͨ����1��2
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ABT_AD7705_SytemCalibFull(uint8_t _ch)
{
    if (_ch == 1)
    {
	    /* У׼CH1 */
	    ABT_AD7705_WriteByte(REG_SETUP | WRITE | CH_1);	/* дͨ�żĴ�������һ����д���üĴ�����ͨ��1 */
	    ABT_AD7705_WriteByte(MD_CAL_FULL | __CH1_GAIN_BIPOLAR_BUF | FSYNC_0);/* ������У׼ */
	    ABT_AD7705_WaitDRDY();	/* �ȴ��ڲ�������� */
    }
    else if (_ch == 2)
    {
	    /* У׼CH2 */
	    ABT_AD7705_WriteByte(REG_SETUP | WRITE | CH_2);	/* дͨ�żĴ�������һ����д���üĴ�����ͨ��1 */
	    ABT_AD7705_WriteByte(MD_CAL_FULL | __CH2_GAIN_BIPOLAR_BUF | FSYNC_0);	/* ������У׼ */
	    ABT_AD7705_WaitDRDY();	/* �ȴ��ڲ�������� */
    }
}

/*
*********************************************************************************************************
*	�� �� ��: ABT_AD7705_ReadAdc1
*	����˵��: ��ͨ��1��2��ADC����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint16_t ABT_AD7705_ReadAdc(uint8_t _ch)
{
    uint8_t i;
    uint16_t read = 0;

    /* Ϊ�˱���ͨ���л���ɶ���ʧЧ����2�� */
    for (i = 0; i < 2; i++)
    {
	    ABT_AD7705_WaitDRDY();		/* �ȴ�DRDY����Ϊ0 */		

	    if (_ch == 1)
	    {
		    ABT_AD7705_WriteByte(0x38);
	    }
	    else if (_ch == 2)
	    {
		    ABT_AD7705_WriteByte(0x39);
	    }

	    read = ABT_AD7705_Read2Byte();
    }
    return read;	
}

/**************************************************************/
