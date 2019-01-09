/*
---------��Զ���ܿƼ����Ϻ������޹�˾---------
---------��Ȩ���� ����ؾ�-------------------
---------2018.07.18 yusengo@163.com---------
*/



#ifndef _MK60_FLASH_H_
#define _MK60_FLASH_H_

//k60N512����512K�ĳ���Flash
//512K�ĳ���Flash��Ϊ256��������ÿ������2K��С
//K60X256����256K�ĳ���Flash
//512K�ĳ���Flash��Ϊ128��������ÿ������2K��С
//    sector��2K��Ϊ������С��λ
//    ���֣�32b��Ϊд����С��λ

#include "common.h"
#include "MainBoard.h"

#if defined(MK60DZ10)
#define     FLASH_SECTOR_SIZE       (2*1024)                //������С Ϊ 2k �ֽ�
#define     FLASH_SECTOR_NUM        (256)                   //������

#define     FLASH_ALIGN_ADDR        4                       //��ַ����������
typedef     uint32                  FLASH_WRITE_TYPE;       //flash_write ����д�� ����������


#elif defined(MK60F15)
#define     FLASH_SECTOR_SIZE       (4*1024)                //������С Ϊ 4k �ֽ�
#define     FLASH_SECTOR_NUM        (128)                   //������

#define     FLASH_ALIGN_ADDR        8                       //��ַ����������
typedef     uint64                  FLASH_WRITE_TYPE;       //flash_write ����д�� ����������

#endif




//little endian
#define GH_FLASH_DATA_SECTORNUM 	255			//LAST page

#define GH_LOAD_JUDGE_ADDR		0
#define ADDR_F_LIST_RHC 		2
#define ADDR_F_LIST_RHIOUT_420_MA 	204
#define ADDR_F_LIST_TIOUT_420_MA 	246

#define ADDR_F_RH_RANGEH	 	288
#define ADDR_F_RH_RANGEL	 	290

#define ADDR_F_T_RANGEH		 	292
#define ADDR_F_T_RANGEL		 	294

#define ADDR_F_TRANSMITTER_SN		296
#define ADDR_F_TRANSMITTER_SW_VERSION	298
#define ADDR_F_TRANSMITTER_TYPE		300
#define ADDR_F_T_P1_COFF		302
#define ADDR_F_T_P2_COFF		306

#define GH_FLASH_DATA_SIZE		800

#define GH_FLASH_FLAG			0X55


/* flash var

*/
typedef struct{
    uint16 F_LIST_RHC[101];  
    uint16 F_LIST_TIOUT_420_MA[21]; 
    uint16 F_LIST_RHIOUT_420_MA[21];
    
    int16 F_RH_RANGEH;
    int16 F_RH_RANGEL;
    int16 F_T_RANGEH;
    int16 F_T_RANGEL;
    
    uint16 F_TRANSMITTER_TYPE;
    uint16 F_TRANSMITTER_SN;
    uint16 F_TRANSMITTER_SW_VERSION;
    float F_T_P1_COFF;
    float F_T_P2_COFF;
}GH_MB_FlashData_Type;

/* global realtime var dont saved to flash
MB_RH 
MB_T 
*/
typedef struct{
    float G_MB_RH;
    float G_MB_T;
    uint16 G_MB_SET_DEBUG_EN;
    uint16 G_MB_SET_HEAT_SELF_TEST;
    uint16 G_MB_RESET_TRANSMITTER;
}GH_MB_GlobalData_Type;

extern uint8 G_FlashDataConvArray[GH_FLASH_DATA_SIZE];

extern GH_MB_FlashData_Type GH_MB_FlashData_t;
extern GH_MB_GlobalData_Type GH_MB_GlobalData_t;

void ABT_FlashInit(void);
uint8 ABT_LoadJudge(void);

void ABT_ParaLoadFromFactory(void);
void ABT_ParaLoadFromFlash(void);
void ABT_ParaSaveToFlash(void);
void ABT_ResetToFactory(void);

__RAMFUNC    void    flash_init();                                                                               //��ʼ��Flash

__RAMFUNC    uint8   flash_erase_sector  (uint16 sectorNo);                                                      //����ָ��flash����

__RAMFUNC    uint8   flash_write         (uint16 sectorNo, uint16 offset, FLASH_WRITE_TYPE data);                //д��flash����
__RAMFUNC    uint8   flash_write_buf     (uint16 sectorNo, uint16 offset, uint16 cnt, uint8 buf[]);              //�ӻ�����д��flash����

#define     flash_read(sectorNo,offset,type)        (*(type *)((uint32)(((sectorNo)*FLASH_SECTOR_SIZE) + (offset))))          //��ȡ����



#endif //_FLASH_H_
