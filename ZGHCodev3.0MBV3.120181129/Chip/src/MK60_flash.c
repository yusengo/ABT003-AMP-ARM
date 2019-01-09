/*
---------��Զ���ܿƼ����Ϻ������޹�˾---------
---------��Ȩ���� ����ؾ�-------------------
---------2018.07.18 yusengo@163.com---------
*/


//����ͷ�ļ�

#include "MK60_flash.h"
#include "MK60_conf.h"

//�궨�� FCCOB Number
#if defined(MK60DZ10)
#define     FCMD        FTFL_FCCOB0         //FTFL ����

#define     FADDR2      FTFL_FCCOB1         //Flash address [23:16]
#define     FADDR1      FTFL_FCCOB2         //Flash address [15:8]
#define     FADDR0      FTFL_FCCOB3         //Flash address [7:0]

#define     FDATA0      FTFL_FCCOB4         //Data Byte 0       //ע��һ�㣬4�ֽ�����FDATA3�����λ��FDATA0�����λ
#define     FDATA1      FTFL_FCCOB5         //Data Byte 1
#define     FDATA2      FTFL_FCCOB6         //Data Byte 2
#define     FDATA3      FTFL_FCCOB7         //Data Byte 3
#define     FDATA4      FTFL_FCCOB8         //Data Byte 4
#define     FDATA5      FTFL_FCCOB9         //Data Byte 5
#define     FDATA6      FTFL_FCCOBA         //Data Byte 6
#define     FDATA7      FTFL_FCCOBB         //Data Byte 7

//FCMD ����
#define     RD1BLK    0x00   // ������Flash
#define     RD1SEC    0x01   // ����������
#define     PGMCHK    0x02   // д����
#define     RDRSRC    0x03   // ��Ŀ������(4�ֽ�)
#define     PGM4      0x06   // д�볤��(4�ֽ�)
#define     ERSBLK    0x08   // ��������Flash
#define     ERSSCR    0x09   // ����Flash����
#define     PGMSEC    0x0B   // д������
#define     RD1ALL    0x40   // �����еĿ�
#define     RDONCE    0x41   // ֻ��һ��
#define     PGMONCE   0x43   // ֻдһ��
#define     ERSALL    0x44   // �������п�
#define     VFYKEY    0x45   // ��֤���ŷ���Կ��
#define     PGMPART   0x80   // д�����
#define     SETRAM    0x81   // �趨FlexRAM����

#elif defined(MK60F15)
#define     FCMD        FTFE_FCCOB0         //FTFL ����

#define     FADDR2      FTFE_FCCOB1         //Flash address [23:16]
#define     FADDR1      FTFE_FCCOB2         //Flash address [15:8]
#define     FADDR0      FTFE_FCCOB3         //Flash address [7:0]
//Must be 64-bit aligned (Flash address [2:0] = 000)  8�ֽڶ���

#define     FDATA0      FTFE_FCCOB4         //Data Byte 0       //ע��һ�㣬4�ֽ�����FDATA3�����λ��FDATA0�����λ
#define     FDATA1      FTFE_FCCOB5         //Data Byte 1
#define     FDATA2      FTFE_FCCOB6         //Data Byte 2
#define     FDATA3      FTFE_FCCOB7         //Data Byte 3
#define     FDATA4      FTFE_FCCOB8         //Data Byte 4
#define     FDATA5      FTFE_FCCOB9         //Data Byte 5
#define     FDATA6      FTFE_FCCOBA         //Data Byte 6
#define     FDATA7      FTFE_FCCOBB         //Data Byte 7

//FCMD ����
#define     RD1BLK    0x00   // ������Flash
#define     RD1SEC    0x01   // ����������
#define     PGMCHK    0x02   // д����
#define     RDRSRC    0x03   // ��Ŀ������(8�ֽ�)
#define     PGM8      0x07   // д�볤��(8�ֽ�)
#define     ERSBLK    0x08   // ��������Flash
#define     ERSSCR    0x09   // ����Flash����
#define     PGMSEC    0x0B   // д������
#define     RD1ALL    0x40   // �����еĿ�
#define     RDONCE    0x41   // ֻ��һ��
#define     PGMONCE   0x43   // ֻдһ��
#define     ERSALL    0x44   // �������п�
#define     VFYKEY    0x45   // ��֤���ŷ���Կ��
#define     PGMPART   0x80   // д�����
#define     SETRAM    0x81   // �趨FlexRAM����

#endif

extern uint8 G_FlashDataConvArray[GH_FLASH_DATA_SIZE] = {0};
extern GH_MB_FlashData_Type GH_MB_FlashData_t = {0};
extern GH_MB_GlobalData_Type GH_MB_GlobalData_t = {0};

/*!
 *  @brief      Flash����
 *  @return     ����ִ�н��(1�ɹ���0ʧ��)
 *  @since      v5.0
 */
__RAMFUNC uint8 flash_cmd()
{
    //д FTFL_FSTAT ���� Flash����
#if defined(MK60DZ10)
    FTFL_FSTAT =    (0
                     |  FTFL_FSTAT_CCIF_MASK        // ָ����ɱ�־(д1��0)
                     |  FTFL_FSTAT_RDCOLERR_MASK    // ����ͻ�����־(д1��0)
                     |  FTFL_FSTAT_ACCERR_MASK      // ���ʴ����־λ(д1��0)
                     |  FTFL_FSTAT_FPVIOL_MASK      // �Ƿ����ʱ�����־λ(д1��0)
                    );

    while(!(FTFL_FSTAT & FTFL_FSTAT_CCIF_MASK));    // �ȴ��������

    // �������־
    if( FTFL_FSTAT & (FTFL_FSTAT_ACCERR_MASK | FTFL_FSTAT_RDCOLERR_MASK | FTFL_FSTAT_FPVIOL_MASK | FTFL_FSTAT_MGSTAT0_MASK))
    {
        return 0 ;                                  //ִ���������
    }
    else
    {
        return 1;                                   //ִ������ɹ�
    }

#elif defined(MK60F15)
    FTFE_FSTAT =    (0
                     |  FTFE_FSTAT_CCIF_MASK        // ָ����ɱ�־(д1��0)
                     |  FTFE_FSTAT_RDCOLERR_MASK    // ����ͻ�����־(д1��0)
                     |  FTFE_FSTAT_ACCERR_MASK      // ���ʴ����־λ(д1��0)
                     |  FTFE_FSTAT_FPVIOL_MASK      // �Ƿ����ʱ�����־λ(д1��0)
                    );

    while(!(FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK));    // �ȴ��������

    // �������־
    if( FTFE_FSTAT & (FTFE_FSTAT_ACCERR_MASK | FTFE_FSTAT_RDCOLERR_MASK | FTFE_FSTAT_FPVIOL_MASK | FTFE_FSTAT_MGSTAT0_MASK))
    {
        return 0 ;                                  //ִ���������
    }
    else
    {
        return 1;                                   //ִ������ɹ�
    }

#endif
}


/*!
 *  @brief      ��ʼ��flash
 *  @since      v5.0
 */
__RAMFUNC void flash_init(void)
{
#if defined( MK60DZ10)
    // ���FlashԤ��ȡ������
    FMC_PFB0CR |= FMC_PFB0CR_S_B_INV_MASK;      //
    FMC_PFB1CR |= FMC_PFB0CR_S_B_INV_MASK;

    while(!(FTFL_FSTAT & FTFL_FSTAT_CCIF_MASK));    // �ȴ��������

    FTFL_FSTAT =    (0
                     |  FTFL_FSTAT_CCIF_MASK        // ָ����ɱ�־(д1��0)
                     |  FTFL_FSTAT_RDCOLERR_MASK    // ����ͻ�����־(д1��0)
                     |  FTFL_FSTAT_ACCERR_MASK      // ���ʴ����־λ(д1��0)
                     |  FTFL_FSTAT_FPVIOL_MASK      // �Ƿ����ʱ�����־λ(д1��0)
                    );

#elif defined(MK60F15)
    // ���FlashԤ��ȡ������
    FMC_PFB01CR |= FMC_PFB01CR_S_B_INV_MASK;      //
    //FMC_PFB23CR |= FMC_PFB23CR_S_B_INV_MASK;

    while(!(FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK));    // �ȴ��������

    FTFE_FSTAT =    (0
                     |  FTFE_FSTAT_CCIF_MASK        // ָ����ɱ�־(д1��0)
                     |  FTFE_FSTAT_RDCOLERR_MASK    // ����ͻ�����־(д1��0)
                     |  FTFE_FSTAT_ACCERR_MASK      // ���ʴ����־λ(д1��0)
                     |  FTFE_FSTAT_FPVIOL_MASK      // �Ƿ����ʱ�����־λ(д1��0)
                    );
#endif
    DELAY_MS(10);
}

/*!
 *  @brief      ����ָ��flash����
 *  @param      sector_num    �����ţ�K60N512ʵ��ʹ��0~255��
 *  @return     ִ�н��(1�ɹ���0ʧ��)
 *  @since      v5.0
 *  Sample usage:       flash_erase_sector(127);        //��������127
 */
__RAMFUNC uint8 flash_erase_sector(uint16 sector_num)
{
    uint32 addr = sector_num * FLASH_SECTOR_SIZE;

    // ���ò�������
    FCMD = ERSSCR;

    // ����Ŀ���ַ
    FADDR2 = ((Dtype *)&addr)->B[2];
    FADDR1 = ((Dtype *)&addr)->B[1];
    FADDR0 = ((Dtype *)&addr)->B[0];

    if(flash_cmd() == 0)
    {
        return 0;
    }

    if(sector_num == 0)
    {
#if defined(MK60DZ10)
        return flash_write(sector_num,0x00040C,0xFFFFFFFE);
#elif defined(MK60F15)
        return flash_write(sector_num,0x000408,0xFFFFFFFEFFFFFFFF );
#endif
    }

    return 1;
}


/*!
 *  @brief      д�볤�ֽ����ݵ� flashָ����ַ
 *  @param      sector_num      �����ţ�0 ~ FLASH_SECTOR_NUM��
 *  @param      offset          д�������ڲ�ƫ�Ƶ�ַ��0~2043 �� 4�ı�����
 *  @param      data            ��Ҫд�������
 *  @return     ִ�н��(1�ɹ���0ʧ��)
 *  @since      v5.0
 *  Sample usage:       flash_write(127,0,0xFFFFFFFE);        //����127,ƫ�Ƶ�ַΪ0,д������:0xFFFFFFFE
 */
__RAMFUNC uint8 flash_write(uint16 sector_num, uint16 offset, FLASH_WRITE_TYPE data)
{
    uint32 addr = sector_num * FLASH_SECTOR_SIZE  + offset ;
    uint32 tmpdata;

    ASSERT(offset % FLASH_ALIGN_ADDR == 0);             //ƫ��������Ϊ4�ı���
    //�˴���ʾ���棬���ǰ�ȫ�ġ���
    ASSERT(offset <= FLASH_SECTOR_SIZE);                //������СΪ2K����ƫ�������벻���� 0x800
    //�˴���ʾ���棬���ǰ�ȫ�ġ���

    // ����Ŀ���ַ
    FADDR2 = ((Dtype *)&addr)->B[2];
    FADDR1 = ((Dtype *)&addr)->B[1];
    FADDR0 = ((Dtype *)&addr)->B[0];

    // ���� ��32λ����
    tmpdata = (uint32)data;

    FDATA0 = ((Dtype *)&tmpdata)->B[3];                    // ����д������
    FDATA1 = ((Dtype *)&tmpdata)->B[2];
    FDATA2 = ((Dtype *)&tmpdata)->B[1];
    FDATA3 = ((Dtype *)&tmpdata)->B[0];

#if defined(MK60DZ10)

    // ���ò�������
    FCMD = PGM4;

#elif defined(MK60F15)

    // ���ò�������
    FCMD = PGM8;

    // ���� ��32λ����
    tmpdata = (uint32)(data>>32);

    FDATA4 = ((Dtype *)&tmpdata)->B[3];                 // ����д������
    FDATA5 = ((Dtype *)&tmpdata)->B[2];
    FDATA6 = ((Dtype *)&tmpdata)->B[1];
    FDATA7 = ((Dtype *)&tmpdata)->B[0];
#endif

    if(flash_cmd() == 0)
    {
        return 0;
    }

    return 1;  //�ɹ�ִ��

}


/*!
 *  @brief      д�����ݻ������� flashָ����ַ
 *  @param      sector_num      �����ţ�K60N512ʵ��ʹ��0~255��
 *  @param      offset          д�������ڲ�ƫ�Ƶ�ַ��0~2043 �� 4�ı�����
 *  @param      buf             ��Ҫд������ݻ������׵�ַ
 *  @return     ִ�н��(1�ɹ���0ʧ��)
 *  @since      v5.0
 *  Sample usage:           uint32 buff[10];
                            flash_write_buf(127,0,sizeof(buff),buff);        //����127,ƫ�Ƶ�ַΪ0,д������Դ��ַ:buff,��Ŀsizeof(buff)
 */

__RAMFUNC uint8 flash_write_buf(uint16 sector_num, uint16 offset, uint16 cnt, uint8 *buf)
{
    uint32  size;

    uint32  addr;
    uint32  data;

    ASSERT(offset % FLASH_ALIGN_ADDR == 0);                    //ƫ��������Ϊ4�ı���
    //�˴���ʾ���棬���ǰ�ȫ��(ram��������rom�������𾯸�)
    ASSERT(offset <= FLASH_SECTOR_SIZE );                    //������СΪ2K����ƫ�������벻���� 0x800
    //�˴���ʾ���棬���ǰ�ȫ��(ram��������rom�������𾯸�)

    addr = sector_num * FLASH_SECTOR_SIZE  + offset;      //�����ַ

    // ����д������
#if defined(MK60DZ10)
    FCMD = PGM4;
#elif defined(MK60F15)
    FCMD = PGM8;
#endif

    for(size = 0; size < cnt; size += FLASH_ALIGN_ADDR )
    {

        // ����Ŀ���ַ
        FADDR2 = ((Dtype *)&addr)->B[2];
        FADDR1 = ((Dtype *)&addr)->B[1];
        FADDR0 = ((Dtype *)&addr)->B[0];

        // ���� ��32λ����
        data =  *(uint32 *)buf;

        FDATA0 = ((Dtype *)&data)->B[3];                    // ����д������
        FDATA1 = ((Dtype *)&data)->B[2];
        FDATA2 = ((Dtype *)&data)->B[1];
        FDATA3 = ((Dtype *)&data)->B[0];

#if defined(MK60F15)
        // ���� ��32λ����
        data = *(uint32 *)(buf+4);

        FDATA4 = ((Dtype *)&data)->B[3];                    // ����д������
        FDATA5 = ((Dtype *)&data)->B[2];
        FDATA6 = ((Dtype *)&data)->B[1];
        FDATA7 = ((Dtype *)&data)->B[0];
#endif

        if(flash_cmd() == 0)
        {
            return 0;
        }

        addr += FLASH_ALIGN_ADDR;
        buf += FLASH_ALIGN_ADDR;
    }
    return 1;  //�ɹ�ִ��
}


uint8 ABT_flash_read(uint16 sectNum,uint16 offset,uint16 cnt,uint8 *resultBuf)
{
    uint32 wAddr = 0;
    
    wAddr = sectNum * 2048 + offset;
    
    while (cnt--)
    {
	*resultBuf = *(uint8*)wAddr;
	resultBuf++;
	wAddr++;
    }
    
    return TRUE;
}

void ABT_FlashInit(void)
{
    flash_init();
}

uint8 ABT_LoadJudge(void)
{  
    if( (*(uint8*)(GH_FLASH_DATA_SECTORNUM * 2048)) == GH_FLASH_FLAG)
	return TRUE;
    else
	return FALSE;
}

void ABT_ConvArraySaveToFlash(void)
{
    flash_erase_sector(GH_FLASH_DATA_SECTORNUM);
    flash_write_buf(GH_FLASH_DATA_SECTORNUM,0, GH_FLASH_DATA_SIZE, G_FlashDataConvArray);
        
    printf("para saved to flash\n");
}

void ABT_FlashLoadToConvArray(void)
{
    ABT_flash_read(GH_FLASH_DATA_SECTORNUM,0, GH_FLASH_DATA_SIZE, G_FlashDataConvArray);
}

void ABT_ParaSaveToFlash(void)
{
    uint8 index = 0;
    G_FlashDataConvArray[GH_LOAD_JUDGE_ADDR]			= GH_FLASH_FLAG;
    G_FlashDataConvArray[GH_LOAD_JUDGE_ADDR + 1]		= 0x00;
    
    for(index = 0;index <= 100;index++)
    {
	G_FlashDataConvArray[ADDR_F_LIST_RHC + (index<<1)]	= (GH_MB_FlashData_t.F_LIST_RHC[index]) & 0xff;
	G_FlashDataConvArray[ADDR_F_LIST_RHC + (index<<1) + 1] 	= (GH_MB_FlashData_t.F_LIST_RHC[index] >> 8) & 0xff;
    }
    
    for(index = 0;index <= 20;index++)
    { 
	G_FlashDataConvArray[ADDR_F_LIST_RHIOUT_420_MA + (index<<1)]		= (GH_MB_FlashData_t.F_LIST_RHIOUT_420_MA[index]) & 0xff;
	G_FlashDataConvArray[ADDR_F_LIST_RHIOUT_420_MA + (index<<1) + 1] 	= (GH_MB_FlashData_t.F_LIST_RHIOUT_420_MA[index] >> 8) & 0xff;
    }
    
    for(index = 0;index <= 20;index++)
    {
	G_FlashDataConvArray[ADDR_F_LIST_TIOUT_420_MA + (index<<1)]		= (GH_MB_FlashData_t.F_LIST_TIOUT_420_MA[index]) & 0xff;
	G_FlashDataConvArray[ADDR_F_LIST_TIOUT_420_MA + (index<<1) + 1]		= (GH_MB_FlashData_t.F_LIST_TIOUT_420_MA[index] >> 8) & 0xff;
    }
    
    memcpy(&(G_FlashDataConvArray[ADDR_F_RH_RANGEH]), &(GH_MB_FlashData_t.F_RH_RANGEH), 2);
    memcpy(&(G_FlashDataConvArray[ADDR_F_RH_RANGEL]),&(GH_MB_FlashData_t.F_RH_RANGEL),  2);

    memcpy(&(G_FlashDataConvArray[ADDR_F_T_RANGEH]), &(GH_MB_FlashData_t.F_T_RANGEH), 2);
    memcpy(&(G_FlashDataConvArray[ADDR_F_T_RANGEL]), &(GH_MB_FlashData_t.F_T_RANGEL), 2);
    
    memcpy(&(G_FlashDataConvArray[ADDR_F_TRANSMITTER_SN]), &(GH_MB_FlashData_t.F_TRANSMITTER_SN), 2);
    memcpy(&(G_FlashDataConvArray[ADDR_F_TRANSMITTER_SW_VERSION]), &(GH_MB_FlashData_t.F_TRANSMITTER_SW_VERSION), 2);
    memcpy(&(G_FlashDataConvArray[ADDR_F_TRANSMITTER_TYPE]), &(GH_MB_FlashData_t.F_TRANSMITTER_TYPE), 2);
    
    memcpy(&(G_FlashDataConvArray[ADDR_F_T_P1_COFF]), &(GH_MB_FlashData_t.F_T_P1_COFF), 4);
    memcpy(&(G_FlashDataConvArray[ADDR_F_T_P2_COFF]), &(GH_MB_FlashData_t.F_T_P2_COFF), 4);  
    
    ABT_ConvArraySaveToFlash();
}

void ABT_ParaLoadFromFlash(void)
{
    uint8 index = 0;
    
    ABT_FlashLoadToConvArray();
    
    for(index = 0;index <= 100;index ++)
	memcpy(&(GH_MB_FlashData_t.F_LIST_RHC[index]),&(G_FlashDataConvArray[ADDR_F_LIST_RHC + (index<<1)]),2); 
	//GH_MB_FlashData_t.F_LIST_RHC[index] 		= (G_FlashDataConvArray[ADDR_F_LIST_RHC + (index<<1) + 1] << 8) | G_FlashDataConvArray[ADDR_F_LIST_RHC + (index<<1)];
    
    for(index = 0;index <= 20;index++)
	memcpy(&(GH_MB_FlashData_t.F_LIST_RHIOUT_420_MA[index]),&(G_FlashDataConvArray[ADDR_F_LIST_TIOUT_420_MA + (index<<1)]),2); 
	//GH_MB_FlashData_t.F_LIST_RHIOUT_420_MA[index] 	= (G_FlashDataConvArray[ADDR_F_LIST_RHIOUT_420_MA + (index<<1) + 1] << 8) | G_FlashDataConvArray[ADDR_F_LIST_RHIOUT_420_MA + (index<<1)];
    
    for(index = 0;index <= 20;index++)
	memcpy(&(GH_MB_FlashData_t.F_LIST_TIOUT_420_MA[index]),&(G_FlashDataConvArray[ADDR_F_LIST_TIOUT_420_MA + (index<<1)]),2); 
	//GH_MB_FlashData_t.F_LIST_TIOUT_420_MA[index] 	= (G_FlashDataConvArray[ADDR_F_LIST_TIOUT_420_MA + (index<<1) + 1] << 8) | G_FlashDataConvArray[ADDR_F_LIST_TIOUT_420_MA + (index<<1)];
        
    memcpy(&(GH_MB_FlashData_t.F_RH_RANGEH), &(G_FlashDataConvArray[ADDR_F_RH_RANGEH]), 2);
    memcpy(&(GH_MB_FlashData_t.F_RH_RANGEL), &(G_FlashDataConvArray[ADDR_F_RH_RANGEL]), 2);

    memcpy(&(GH_MB_FlashData_t.F_T_RANGEH), &(G_FlashDataConvArray[ADDR_F_T_RANGEH]), 2);
    memcpy(&(GH_MB_FlashData_t.F_T_RANGEL), &(G_FlashDataConvArray[ADDR_F_T_RANGEL]), 2);
    
    memcpy(&(GH_MB_FlashData_t.F_TRANSMITTER_SN), &(G_FlashDataConvArray[ADDR_F_TRANSMITTER_SN]), 2);
    memcpy(&(GH_MB_FlashData_t.F_TRANSMITTER_SW_VERSION), &(G_FlashDataConvArray[ADDR_F_TRANSMITTER_SW_VERSION]), 2);
    memcpy(&(GH_MB_FlashData_t.F_TRANSMITTER_TYPE), &(G_FlashDataConvArray[ADDR_F_TRANSMITTER_TYPE]), 2);
    
    memcpy(&(GH_MB_FlashData_t.F_T_P1_COFF), &(G_FlashDataConvArray[ADDR_F_T_P1_COFF]), 4);
    memcpy(&(GH_MB_FlashData_t.F_T_P2_COFF), &(G_FlashDataConvArray[ADDR_F_T_P2_COFF]), 4);
       
    printf("para loaded from flash\n");
}

void ABT_ParaLoadFromFactory(void)
{
    uint8 index = 0;
    
    for(index = 0;index <= 100;index++)
	GH_MB_FlashData_t.F_LIST_RHC[index] 		= FACTORY_LIST_RHC[index];
    
    for(index = 0;index <= 20;index++)
	GH_MB_FlashData_t.F_LIST_RHIOUT_420_MA[index] 	= FACTORY_LIST_RHIOUT_420_MA[index];
    
    for(index = 0;index <= 20;index++)
	GH_MB_FlashData_t.F_LIST_TIOUT_420_MA[index] 	= FACTORY_LIST_TIOUT_420_MA[index];
    
    GH_MB_FlashData_t.F_RH_RANGEH 		= 100;
    GH_MB_FlashData_t.F_RH_RANGEL 		= 0;
    
    GH_MB_FlashData_t.F_T_RANGEH 		= 200;
    GH_MB_FlashData_t.F_T_RANGEL 		= 0;
    
    GH_MB_FlashData_t.F_TRANSMITTER_SN		= 1;
    GH_MB_FlashData_t.F_TRANSMITTER_SW_VERSION 	= 3;
    GH_MB_FlashData_t.F_TRANSMITTER_TYPE 	= 1;
    
    GH_MB_FlashData_t.F_T_P1_COFF 		= 0.2486;
    GH_MB_FlashData_t.F_T_P2_COFF 		= - 279.4;
    
    printf("para loaded from factory\n");
}

void ABT_ResetToFactory(void)
{
    uint8 index = 0;
	
    ABT_ParaLoadFromFactory();
    
    G_FlashDataConvArray[GH_LOAD_JUDGE_ADDR]			= 0x00;
    G_FlashDataConvArray[GH_LOAD_JUDGE_ADDR + 1]		= 0x00;
    
    for(index = 0;index <= 20;index++)
    { 
	G_FlashDataConvArray[ADDR_F_LIST_RHIOUT_420_MA + (index<<1)]		= (GH_MB_FlashData_t.F_LIST_RHIOUT_420_MA[index]) & 0xff;
	G_FlashDataConvArray[ADDR_F_LIST_RHIOUT_420_MA + (index<<1) + 1] 	= (GH_MB_FlashData_t.F_LIST_RHIOUT_420_MA[index] >> 8) & 0xff;
    }
    
    for(index = 0;index <= 20;index++)
    {
	G_FlashDataConvArray[ADDR_F_LIST_TIOUT_420_MA + (index<<1)]		= (GH_MB_FlashData_t.F_LIST_TIOUT_420_MA[index]) & 0xff;
	G_FlashDataConvArray[ADDR_F_LIST_TIOUT_420_MA + (index<<1) + 1]		= (GH_MB_FlashData_t.F_LIST_TIOUT_420_MA[index] >> 8) & 0xff;
    }
    
    memcpy(&(G_FlashDataConvArray[ADDR_F_RH_RANGEH]), &(GH_MB_FlashData_t.F_RH_RANGEH), 2);
    memcpy(&(G_FlashDataConvArray[ADDR_F_RH_RANGEL]),&(GH_MB_FlashData_t.F_RH_RANGEL),  2);

    memcpy(&(G_FlashDataConvArray[ADDR_F_T_RANGEH]), &(GH_MB_FlashData_t.F_T_RANGEH), 2);
    memcpy(&(G_FlashDataConvArray[ADDR_F_T_RANGEL]), &(GH_MB_FlashData_t.F_T_RANGEL), 2);
    
    memcpy(&(G_FlashDataConvArray[ADDR_F_TRANSMITTER_SN]), &(GH_MB_FlashData_t.F_TRANSMITTER_SN), 2);
    memcpy(&(G_FlashDataConvArray[ADDR_F_TRANSMITTER_SW_VERSION]), &(GH_MB_FlashData_t.F_TRANSMITTER_SW_VERSION), 2);
    memcpy(&(G_FlashDataConvArray[ADDR_F_TRANSMITTER_TYPE]), &(GH_MB_FlashData_t.F_TRANSMITTER_TYPE), 2);
    
    memcpy(&(G_FlashDataConvArray[ADDR_F_T_P1_COFF]), &(GH_MB_FlashData_t.F_T_P1_COFF), 4);
    memcpy(&(G_FlashDataConvArray[ADDR_F_T_P2_COFF]), &(GH_MB_FlashData_t.F_T_P2_COFF), 4); 
    
    
    ABT_ConvArraySaveToFlash();
        
    printf("para ResetToFactory\n");
}
