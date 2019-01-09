#include "common.h"
#ifndef GEMCREATE_PROBE_H_
#define GEMCREATE_PROBE_H_
#include "MainBoard.h"
#include "MK60_flash.h"
#define MAX_SENSOR_NUM 1

//CMD DESCRIPTION
#define VOLTAGE_GENERATE        0x20
#define DEVICE_SELF_TEST		0x21
#define SET_CALIB_POINT			0x22
#define CLEAR_CALIB_STATUS      0x23
#define GET_PROBE_RH			0x24

//PUBLIC FOR USER
#define GET_INSPECT_DATA		0x59
#define GET_PROBE_TYPE			0x60
#define GET_PRODUCT_TYPE		0x61
#define GET_PROBE_SN			0x62
#define GET_PROBE_SW_VERSION		0x63
#define GET_RH					0x64
#define GET_T					0x65
#define GET_TD					0x66
#define GET_TF					0x67
#define GET_TW					0x68
#define GET_E					0x69
#define GET_R					0x6A
#define GET_DV					0x6B
#define GET_H					0x6C
#define GET_RH_RANGE			0x6D
#define GET_T_RANGE			0x6E
#define GET_TD_RANGE			0x6F
#define GET_TF_RANGE			0x70
#define GET_TW_RANGE			0x71
#define GET_E_RANGE			0x72
#define GET_R_RANGE			0x73
#define GET_DV_RANGE			0x74
#define GET_H_RANGE			0x75
#define SET_RH_RANGE			0x76
#define SET_T_RANGE			0x77
#define SET_TD_RANGE			0x78
#define SET_TF_RANGE			0x79
#define SET_TW_RANGE			0x7A
#define SET_E_RANGE			0x7B
#define SET_R_RANGE			0x7C
#define SET_DV_RANGE			0x7D
#define SET_H_RANGE			0x7E

//PRIVATE FOR FACTORY TEST
#define SET_PRODUCT_TYPE		0x80
#define SET_PROBE_SN			0x81
#define GET_K_VALUE			0x82
#define GET_MX_VALUE			0x83
#define SET_RH_COEF				0x84
#define SET_T_COEF				0x85
#define SET_TD_COEF				0x86
#define SET_TF_COEF				0x87
#define SET_TW_COEF				0x88
#define SET_E_COEF				0x89
#define SET_R_COEF				0x8A
#define SET_DV_COEF				0x8B
#define SET_H_COEF				0x8C
#define SET_PWM1_COEF			0x8D
#define SET_PWM2_COEF			0x8E
#define SET_PWM3_COEF			0x8F
#define SET_PWM_DUTY_OUT		0x90
#define GET_ALL_PARA	    	0x91
#define WRITE_ALL_PARA	    	0x92
#define RESET_PROBE				0x93

#define GET_CH1_K               0x94    //add by xujun 2016-11-24,for pwm debug
#define GET_CH1_B               0x95    //add by xujun 2016-11-24,for pwm debug
#define GET_CH2_K               0x96    //add by xujun 2016-11-24,for pwm debug
#define GET_CH2_B               0x97    //add by xujun 2016-11-24,for pwm debug
#define GET_CH1_DUTY            0x98    //add by xujun 2016-11-24,for pwm debug
#define GET_CH2_DUTY            0x99    //add by xujun 2016-11-24,for pwm debug
#define GET_RH_COEF             0x9A    //add by xujun 2016-12-21,for 2point calib
#define GET_T_COEF              0x9B    //add by xujun 2016-12-21,for 2point calib
#define GET_CH3_K               0x9C
#define GET_CH3_B               0x9D
#define GET_CH3_DUTY            0x9E
#define SET_KALMAN_EN           0x9F    //add by xujun 2017-05-24,for kalman filter
#define GET_KALMAN_EN           0xA0    //add by xujun 2017-05-24,for kalman filter  

//FIRMWARE UPDATE BY IIC PORT
#define UPDATE_PROBE_FIRMWARE	0x5A
#define UPDATE_PROBE_FIRMWARE_D 0x5B
#define GET_PROBE_DATA_DIRECT   0x5C

#define CMD_DUMMY				0x00


#define PROBE_VALID                     0x11
#define PROBE_INVALID                   0x22


#define GET_TRANSMITTER_TYPE            0x25
#define GET_TRANSMITTER_SN              0x26
#define GET_TRANSMITTER_SW_VERSION      0x27
#define SET_CHN1_OUT                    0x28
#define SET_CHN2_OUT                    0x29
#define SET_CHN3_OUT                    0x2A

#define RESET_TRANSMITTER               0x2B
#define SET_TRANSMITTER_TYPE            0x2C
#define SET_TRANSMITTER_SN              0x2D


#define UPDATE_TRANSMITTER_FIRMWARE     0x5D
#define UPDATE_TRANSMITTER_FIRMWARE_D   0x5E
#define GET_TRANSMITTER_DATA_DIRECT     0x5F

//#define PROBE_ID                        p[1]
#define PROBE_ID                        0
#define CMD_DESCRIPTION                 get_cmd_desc(p[0],p[2])
#define CMD_DATA_START                  2
#define CMD_WDATA_START                 5
typedef struct _dictEntry
{
   unsigned char   	cmd_key;          						    // key
   void (*pFunc)    ( unsigned char* p,unsigned short uLen);    // function value
} CMD_DICT_ENTRY,*PCMD_DICT_ENTRY;

typedef struct tf_data
{
    float d1;
    float d2;
}STRU_TF_DATA,*PSTRU_TF_DATA;


void dth01_iic_dummy();
CMD_DICT_ENTRY pc2board_cmd_dict_run(unsigned char cmd_desc);
void cmd_voltage_generate(unsigned char *p, unsigned short uLen);
void cmd_device_self_test(unsigned char *p, unsigned short uLen);
void cmd_set_calib_point(unsigned char *p, unsigned short uLen);
void cmd_clear_calib_status(unsigned char *p, unsigned short uLen);
void cmd_write_data(unsigned char *p, unsigned short uLen);
void cmd_read_data(unsigned char *p, unsigned short uLen);
void cmd_read_probe_rh(unsigned char* p,unsigned short uLen);
void cmd_update_firmware_d(unsigned char* p,unsigned short uLen);
void cmd_read_data_direct(unsigned char* p,unsigned short uLen);
unsigned char get_cmd_desc(unsigned char uFunCode,unsigned char offset);
void cmd_read_all_para(unsigned char* p,unsigned short uLen);


#endif