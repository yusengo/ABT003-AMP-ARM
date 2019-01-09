
#include "stdlib.h"
#include "string.h"

#include "gemcreate_probe_cmd.h"

void ABT_LEConvBE(uint8 *p,uint8 len)
{
    uint8 temp = 0;
   
    if(len == 2)
    {
	temp = p[0];
	p[0] = p[1];
	p[1] = temp;
    }
    
    if(len == 4)
    {
	temp = p[0];
	p[0] = p[3];
	p[3] = temp;	
	
	temp = p[1];
	p[1] = p[2];
	p[2] = temp;	
    }
}

void cmd_voltage_generate(unsigned char *p,unsigned short uLen)
{

}

void cmd_device_self_test(unsigned char *p, unsigned short uLen)
{

}


void cmd_set_calib_point(unsigned char *p, unsigned short uLen)
{

}


void cmd_clear_calib_status(unsigned char *p, unsigned short uLen)
{
    //calib_point_num = 0;
}



void cmd_write_data(unsigned char *p, unsigned short uLen)
{
	if (NULL == p) {
		return;
	}

}

void cmd_write_data_F_DEBUG420MAEN(unsigned char *p, unsigned short uLen)
{
	if(NULL == p)
		return;

	if(uLen == 0)
	    	return;	
	
	ABT_LEConvBE(&(p[6]),2);
	memcpy(&(GH_MB_GlobalData_t.G_MB_SET_DEBUG_EN),&(p[6]),2);
	
	if(GH_MB_GlobalData_t.G_MB_SET_DEBUG_EN > 20)
	    GH_MB_GlobalData_t.G_MB_SET_DEBUG_EN = 20;	
}

void cmd_write_data_F_ResetFactory(unsigned char *p, unsigned short uLen)
{
	if (NULL == p)
		return;
	if(uLen == 0)
	    	return;
	
	if(((p[6] << 8)|p[7]) == 1)
	    ABT_ResetToFactory();
}

void cmd_write_data_F_SetTransmitterType(unsigned char *p, unsigned short uLen)
{
	if (NULL == p)
		return;
	if(uLen == 0)
	    	return;
	
	ABT_LEConvBE(&(p[6]),2);
	memcpy(&(GH_MB_FlashData_t.F_TRANSMITTER_TYPE),&(p[6]),2);
	
	ABT_ParaSaveToFlash();	
}

void cmd_write_data_F_SetTransmitterSN(unsigned char *p, unsigned short uLen)
{
	if (NULL == p)
		return;
	if(uLen == 0)
	    	return;
	
	ABT_LEConvBE(&(p[6]),2);
	memcpy(&(GH_MB_FlashData_t.F_TRANSMITTER_SN),&(p[6]),2);
	
	ABT_ParaSaveToFlash();	
}

void cmd_write_data_F_SetTransmitterSWV(unsigned char *p, unsigned short uLen)
{
	if (NULL == p)
		return;
	if(uLen == 0)
	    	return;
	
	ABT_LEConvBE(&(p[6]),2);
	memcpy(&(GH_MB_FlashData_t.F_TRANSMITTER_SW_VERSION),&(p[6]),2);
	
	ABT_ParaSaveToFlash();	
}

void cmd_write_data_F_SetTP1coff(unsigned char *p, unsigned short uLen)
{
	if (NULL == p)
		return;
	if(uLen != 4)
	    	return;
	
	ABT_LEConvBE(&(p[6]),4);
	memcpy(&(GH_MB_FlashData_t.F_T_P1_COFF),&(p[6]),4);
	
	ABT_ParaSaveToFlash();	
}

void cmd_write_data_F_SetTP2coff(unsigned char *p, unsigned short uLen)
{
	if (NULL == p)
		return;
	if(uLen != 4)
	    	return;
	
	ABT_LEConvBE(&(p[6]),4);
	memcpy(&(GH_MB_FlashData_t.F_T_P2_COFF),&(p[6]),4);
	
	ABT_ParaSaveToFlash();	
}

void cmd_write_data_TRange(unsigned char *p, unsigned short uLen)
{
	if (NULL == p)
		return;
	if(uLen != 4)
	    	return;

	ABT_LEConvBE(&(p[6]),2);
	memcpy(&(GH_MB_FlashData_t.F_T_RANGEH),&(p[6]),2);
	
	ABT_LEConvBE(&(p[8]),2);
	memcpy(&(GH_MB_FlashData_t.F_T_RANGEL),&(p[8]),2);
	
	if(GH_MB_FlashData_t.F_T_RANGEL >= GH_MB_FlashData_t.F_T_RANGEH)
	{
	    GH_MB_FlashData_t.F_T_RANGEH = 200;
	    GH_MB_FlashData_t.F_T_RANGEL = 0; 
	}
	
	ABT_ParaSaveToFlash();
}

void cmd_write_data_RHRange(unsigned char *p, unsigned short uLen)
{
	if (NULL == p)
		return;
	if(uLen != 4)
	    	return;
	
	ABT_LEConvBE(&(p[6]),2);	
	memcpy(&(GH_MB_FlashData_t.F_RH_RANGEH),&(p[6]),2);

	ABT_LEConvBE(&(p[8]),2);
	memcpy(&(GH_MB_FlashData_t.F_RH_RANGEL),&(p[8]),2);
	
	if(GH_MB_FlashData_t.F_RH_RANGEH >= GH_MB_FlashData_t.F_RH_RANGEH)
	{
	    GH_MB_FlashData_t.F_RH_RANGEH = 100;
	    GH_MB_FlashData_t.F_RH_RANGEH = 0;
	}	
	
	ABT_ParaSaveToFlash();
}

void cmd_write_data_HEAT(unsigned char *p, unsigned short uLen)
{
	if (NULL == p)
		return;
	if(uLen != 2)
	    	return;
	
	ABT_LEConvBE(&(p[6]),2);	
	memcpy(&(GH_MB_GlobalData_t.G_MB_SET_HEAT_SELF_TEST),&(p[6]),2);
}

void cmd_write_data_F_FRHCList(unsigned char *p, unsigned short uLen)
{	
    	uint8 index;
	
	if (p == NULL)
		return;
	if (uLen != 202) 
		return;
	
	for(index = 0;index <= 100;index++)
	{
	    ABT_LEConvBE(&(p[(index<<1) + 6]),2); 
	    memcpy(&(GH_MB_FlashData_t.F_LIST_RHC[index]), &(p[(index<<1) + 6]), 2);
	}
}

void cmd_write_data_F_FRHIOUT420MAList(unsigned char *p, unsigned short uLen)
{	
    	uint8 index;
	
	if (p == NULL)
		return;
	if (uLen != 34) 
		return;
	
	for(index = 0;index <= 16;index++)
	{
	    ABT_LEConvBE(&(p[((index<<1) + 6) + 2]),2); 
	    memcpy(&(GH_MB_FlashData_t.F_LIST_RHIOUT_420_MA[index + 4]), &(p[(index<<1) + 6]), 2);
	}
}

void cmd_write_data_F_FTIOUT420MAList(unsigned char *p, unsigned short uLen)
{	
    	uint8 index;
	
	if (p == NULL)
		return;
	if (uLen != 34) 
		return;
	
	for(index = 0;index <= 16;index++)
	{
	    ABT_LEConvBE(&(p[(index<<1) + 6]),2); 
	    memcpy(&(GH_MB_FlashData_t.F_LIST_TIOUT_420_MA[index + 4]), &(p[(index<<1) + 6]), 2);
	}
}

void cmd_read_data_F_FRHCList(unsigned char *p, unsigned short uLen)
{	
    	uint8 index;
	
	if (p == NULL)
		return;
	if (uLen != 202) 
		return;
	
	p[1] = uLen;
	
	for(index = 0;index <= 100;index++)
	{
	    memcpy(&(p[(index<<1) + 2]), &(GH_MB_FlashData_t.F_LIST_RHC[index]), 2);
	    ABT_LEConvBE(&(p[(index<<1) + 2]),2); 
	}
}

void cmd_read_data_F_FRHIOUT420MAList(unsigned char *p, unsigned short uLen)
{	
    	uint8 index;
	
	if (p == NULL)
		return;
	if (uLen != 34) 
		return;
	
	p[1] = uLen;
	
	for(index = 0;index <= 16;index++)
	{
	    memcpy(&(p[(index<<1) + 2]), &(GH_MB_FlashData_t.F_LIST_RHIOUT_420_MA[index + 4]), 2);
	    ABT_LEConvBE(&(p[(index<<1) + 2]),2); 
	}
}

void cmd_read_data_F_FTIOUT420MAList(unsigned char *p, unsigned short uLen)
{	
    	uint8 index;
	
	if (p == NULL)
		return;
	if (uLen != 34) 
		return;
	
	p[1] = uLen;
	
	for(index = 0;index <= 16;index++)
	{
	    memcpy(&(p[(index<<1) + 2]), &(GH_MB_FlashData_t.F_LIST_TIOUT_420_MA[index + 4]), 2);
	    ABT_LEConvBE(&(p[(index<<1) + 2]),2); 
	}
}

void cmd_read_data_F_getDEBUG420MAEN(unsigned char *p, unsigned short uLen)
{	
	if (p == NULL)
		return;
	if (uLen != 2) 
		return;
	
	p[1] = uLen;
	
	memcpy(&(p[2]), &(GH_MB_GlobalData_t.G_MB_SET_DEBUG_EN), 2); 
	ABT_LEConvBE(&(p[2]),2); 	

}

void cmd_read_data_getT(unsigned char *p, unsigned short uLen)
{	
	if (p == NULL)
		return;
	if (uLen != 4) 
		return;
	
	p[1] = uLen;
	
	
	memcpy(&(p[2]), &(GH_MB_GlobalData_t.G_MB_T), 4); 
	ABT_LEConvBE(&(p[2]),4); 
	    
	//printf("T %d\n",(GH_MB_GlobalData_t.G_MB_T)*100);
}

void cmd_read_data_getRH(unsigned char *p, unsigned short uLen)
{	
	if (p == NULL)
		return;
	if (uLen != 4) 
		return;
	
	p[1] = uLen;
	memcpy(&(p[2]), &(GH_MB_GlobalData_t.G_MB_RH), 4); 
	ABT_LEConvBE(&(p[2]),4);    
	//printf("RH %d\n",(uint16)((GH_MB_GlobalData_t.G_MB_RH)*100));
}

void cmd_read_data_F_getTP1coff(unsigned char *p, unsigned short uLen)
{	
	
	if (p == NULL)
		return;
	if (uLen != 4) 
		return;
	
	p[1] = uLen;
	
	memcpy(&(p[2]), &(GH_MB_FlashData_t.F_T_P1_COFF), 4);
	ABT_LEConvBE(&(p[2]),4); 
}

void cmd_read_data(unsigned char *p, unsigned short uLen)
{		
	if (p == NULL)
		return;
	if (uLen != 4) 
		return;
	
	p[1] = uLen;
	
}

void cmd_read_data_F_getTP2coff(unsigned char *p, unsigned short uLen)
{	
	
	if (p == NULL)
		return;
	if (uLen != 4) 
		return;
	
	p[1] = uLen;
	
	memcpy(&(p[2]), &(GH_MB_FlashData_t.F_T_P2_COFF), 4);
	ABT_LEConvBE(&(p[2]),4); 
}

void cmd_read_data_getRHRange(unsigned char *p, unsigned short uLen)
{
	if (p == NULL)
		return;
	if (uLen != 4) 
		return;
	
	p[1] = uLen;
	
	memcpy(&(p[2]),&(GH_MB_FlashData_t.F_RH_RANGEH),2);
	ABT_LEConvBE(&(p[2]),2);	

	memcpy(&(p[4]),&(GH_MB_FlashData_t.F_RH_RANGEL),2);
	ABT_LEConvBE(&(p[4]),2);
}

void cmd_read_data_getTRange(unsigned char *p, unsigned short uLen)
{
	if (p == NULL)
		return;
	if (uLen != 4) 
		return;
	
	p[1] = uLen;
	
	memcpy(&(p[2]),&(GH_MB_FlashData_t.F_T_RANGEH),2);
	ABT_LEConvBE(&(p[2]),2);	

	memcpy(&(p[4]),&(GH_MB_FlashData_t.F_T_RANGEL),2);
	ABT_LEConvBE(&(p[4]),2);	
}

void cmd_read_data_getInspectData(unsigned char *p, unsigned short uLen)
{
	if (p == NULL)
		return;
	if (uLen != 22) 
		return;
	
	p[1] = uLen;

	memcpy(&(p[2]),&(adcValue),4);
	ABT_LEConvBE(&(p[2]),4);
	
	memcpy(&(p[6]),&(adcRefValue),4);
	ABT_LEConvBE(&(p[6]),4);
	
	memcpy(&(p[10]),&(pTomValue),4);
	ABT_LEConvBE(&(p[10]),4);
	
	memcpy(&(p[14]),&(meaPlsCnt),4);
	ABT_LEConvBE(&(p[14]),4);
	
	memcpy(&(p[18]),&(refPlsCnt),4);
	ABT_LEConvBE(&(p[18]),4);
	
	memcpy(&(p[22]),&(meaTRatio),2);
	ABT_LEConvBE(&(p[22]),2);
}

void cmd_read_data_getTransmitterType(unsigned char *p, unsigned short uLen)
{
	if (p == NULL)
		return;
	if (uLen != 2) 
		return;
	
	p[1] = uLen;
	
	memcpy(&(p[2]),&(GH_MB_FlashData_t.F_TRANSMITTER_TYPE),2);
	ABT_LEConvBE(&(p[2]),2);	
}

void cmd_read_data_getHEAT(unsigned char *p, unsigned short uLen)
{
	if (p == NULL)
		return;
	if (uLen != 2) 
		return;
	
	p[1] = uLen;
	
	memcpy(&(p[2]),&(GH_MB_GlobalData_t.G_MB_SET_HEAT_SELF_TEST),2);
	ABT_LEConvBE(&(p[2]),2);	
}

void cmd_read_data_getTransmitterSN(unsigned char *p, unsigned short uLen)
{
	if (p == NULL)
		return;
	if (uLen != 2) 
		return;
	
	p[1] = uLen;
	
	memcpy(&(p[2]),&(GH_MB_FlashData_t.F_TRANSMITTER_SN),2);
	ABT_LEConvBE(&(p[2]),2);	
}

void cmd_read_data_getTransmitterSWV(unsigned char *p, unsigned short uLen)
{
	if (p == NULL)
		return;
	if (uLen != 2) 
		return;
	
	p[1] = uLen;
	
	memcpy(&(p[2]),&(GH_MB_FlashData_t.F_TRANSMITTER_SW_VERSION),2);
	ABT_LEConvBE(&(p[2]),2);	
}

void cmd_read_probe_rh(unsigned char* p,unsigned short uLen)
{
  
}

void cmd_update_firmware_d(unsigned char* p,unsigned short uLen)
{
    /*select_channel(PROBE_ID);
	dth01_iic_dummy();
	write_data_to_updata_firmware(&p[CMD_WDATA_START],uLen);*/
}


void cmd_read_data_direct(unsigned char* p,unsigned short uLen)
{
    /*select_channel(PROBE_ID);
	dth01_iic_dummy();
    read_data_to_update_firmware(&p[CMD_DATA_START]);
    read_data_to_update_firmware(&p[CMD_DATA_START + 1]);*/
	unsigned char i = 0;
	unsigned char testData = 0x11;
	if (p == NULL) {
		return;
	}

	if (uLen <= 0) {
		return;
	}
    //int chn = PROBE_ID;
	//select_channel(PROBE_ID);
	//dth01_iic_dummy();
	//read_data_from_probe(&p[CMD_DATA_START], CMD_DESCRIPTION, uLen,0);
    p[1] = uLen;
    for(i = 0;i < uLen;i++)
    {
	p[i + 2] = testData++;
    }
    printf("fun is called\n");
}


void cmd_read_all_para(unsigned char* p,unsigned short uLen)
{
    /*select_channel(PROBE_ID);
    dth01_iic_dummy();
    read_data_from_probe(&p[CMD_DATA_START], CMD_DESCRIPTION, uLen,p[3]);*/
    p[1] = uLen;
}

const CMD_DICT_ENTRY pc2board_cmd_list[] =
{
{VOLTAGE_GENERATE	,	cmd_voltage_generate,	},
//calib flow cmd
{DEVICE_SELF_TEST	,	cmd_device_self_test,	},
{SET_CALIB_POINT    ,	cmd_set_calib_point,	},
{CLEAR_CALIB_STATUS ,	cmd_clear_calib_status,	},
{GET_PROBE_RH      ,	cmd_read_probe_rh,		},

{UPDATE_PROBE_FIRMWARE	,	cmd_write_data,         },
{UPDATE_PROBE_FIRMWARE_D,	cmd_update_firmware_d,  },
{GET_PROBE_DATA_DIRECT	,	cmd_read_data_direct,   },

//PUBLIC FOR USER       //PUBLIC FOR USER
{GET_INSPECT_DATA    ,	cmd_read_data_getInspectData,	        	},
{GET_PROBE_TYPE    ,	cmd_read_data_getTransmitterType,},
{GET_PRODUCT_TYPE  ,	cmd_read_data,			},
{GET_PROBE_SN      ,	cmd_read_data_getTransmitterSN,			},
{GET_PROBE_SW_VERSION,	cmd_read_data_getTransmitterSWV,			},
{GET_RH            ,	cmd_read_data_getRH,		},
{GET_T             ,	cmd_read_data_getT,		},
{GET_TD            ,	cmd_read_data,			},
{GET_TF            ,	cmd_read_data,			},
{GET_TW            ,	cmd_read_data,			},
{GET_E             ,	cmd_read_data,			},
{GET_R             ,	cmd_read_data,			},
{GET_DV            ,	cmd_read_data,			},
{GET_H             , 	cmd_read_data,			},
{GET_RH_RANGE      ,	cmd_read_data_getRHRange,	},
{GET_T_RANGE       ,	cmd_read_data_getTRange,	},
{GET_TD_RANGE      ,	cmd_read_data,			},
{GET_TF_RANGE      ,	cmd_read_data,			},
{GET_TW_RANGE      ,	cmd_read_data,			},
{GET_E_RANGE       ,	cmd_read_data,			},
{GET_R_RANGE       ,	cmd_read_data,			},
{GET_DV_RANGE      ,	cmd_read_data,			},
{GET_H_RANGE       ,	cmd_read_data_getHEAT,		},
{SET_RH_RANGE       ,	cmd_write_data_RHRange,         },
{SET_T_RANGE        ,	cmd_write_data_TRange,          },
{SET_TD_RANGE       ,	cmd_write_data,         },
{SET_TF_RANGE       ,	cmd_write_data,         },
{SET_TW_RANGE       ,	cmd_write_data,         },
{SET_E_RANGE        ,	cmd_write_data,         },
{SET_R_RANGE        ,	cmd_write_data,         },
{SET_DV_RANGE       ,	cmd_write_data,         },
{SET_H_RANGE        ,	cmd_write_data_HEAT,         },

//PRIVATE FOR FACTORY TEST  //PRIVATE FOR FACTORY TEST
{SET_PRODUCT_TYPE   , 	cmd_write_data_F_SetTransmitterType,	},
{SET_PROBE_SN       ,	cmd_write_data_F_SetTransmitterSN,         },
{GET_K_VALUE       ,  	cmd_read_data,         	},
{GET_MX_VALUE      , 	cmd_read_data,        	},
{SET_RH_COEF        ,  	cmd_write_data_F_SetTP1coff,         },
{SET_T_COEF         ,	cmd_write_data_F_SetTP2coff,         },
{SET_TD_COEF        ,	cmd_write_data,         },
{SET_TF_COEF        ,	cmd_write_data,         },
{SET_TW_COEF        ,	cmd_write_data,         },
{SET_E_COEF         ,	cmd_write_data,         },
{SET_R_COEF         ,	cmd_write_data,         },
{SET_DV_COEF        ,	cmd_write_data,         },
{SET_H_COEF         ,	cmd_write_data,         },
{SET_PWM1_COEF      ,	cmd_write_data_F_FRHCList,        },
{SET_PWM2_COEF      ,	cmd_write_data_F_FRHIOUT420MAList, },
{SET_PWM3_COEF      ,	cmd_write_data_F_FTIOUT420MAList, },
{SET_PWM_DUTY_OUT   ,	cmd_write_data_F_SetTransmitterSWV,     	},
{GET_ALL_PARA      ,	cmd_read_all_para,     	},
//{WRITE_ALL_PARA     ,	cmd_write_data,       	},
{RESET_PROBE        ,   cmd_write_data_F_ResetFactory,},

{GET_CH1_K         ,   cmd_read_data,          },
{GET_CH1_B         ,   cmd_read_data_F_FRHCList,          },
{GET_CH2_K         ,   cmd_read_data_F_FRHIOUT420MAList,          },
{GET_CH2_B         ,   cmd_read_data_F_FTIOUT420MAList,          },
{GET_CH1_DUTY      ,   cmd_read_data,          },
{GET_CH2_DUTY      ,   cmd_read_data,          },
{GET_RH_COEF       ,   cmd_read_data_F_getTP1coff,          },
{GET_T_COEF        ,   cmd_read_data_F_getTP2coff,          },

{SET_KALMAN_EN     ,   cmd_write_data_F_DEBUG420MAEN,    },
{GET_KALMAN_EN     ,   cmd_read_data_F_getDEBUG420MAEN,  },
};

double calib_record[MAX_SENSOR_NUM][5][4] = { 0 };
const unsigned char USER_GET_CMD[] =
{
    0//offset0
    ,GET_PROBE_TYPE //1
    ,GET_PROBE_SN //2
    ,GET_PROBE_SW_VERSION//3
    ,GET_RH//4
    ,GET_T//5
    ,GET_TD//6
    ,GET_TF//7
    ,GET_TW//8
    ,GET_E//9
    ,GET_R//10
    ,GET_DV//11
    ,GET_H//12
    ,GET_RH_RANGE//13
    ,GET_T_RANGE//14
    ,GET_TD_RANGE//15
    ,GET_TF_RANGE//16
    ,GET_TW_RANGE//17
    ,GET_E_RANGE//18
    ,GET_R_RANGE//19
    ,GET_DV_RANGE//20
    ,GET_H_RANGE//21
    ,GET_TRANSMITTER_TYPE//22
    ,GET_TRANSMITTER_SN//23
    ,GET_TRANSMITTER_SW_VERSION//24
    ,GET_INSPECT_DATA//25
};

const unsigned char USER_SET_CMD[] = {
    0//offset 0
    ,SET_RH_RANGE//1
    ,SET_T_RANGE//2
    ,SET_TD_RANGE//3
    ,SET_TF_RANGE//4
    ,SET_TW_RANGE//5
    ,SET_E_RANGE//6
    ,SET_R_RANGE//7
    ,SET_DV_RANGE//8
    ,SET_H_RANGE//9
    ,SET_CHN1_OUT//10
    ,SET_CHN2_OUT//11
    ,SET_CHN3_OUT//12
};

const unsigned char FACTORY_GET_CMD[] = {
    0
    ,GET_K_VALUE
    ,GET_MX_VALUE
    ,GET_RH_COEF
    ,GET_T_COEF
    ,GET_CH1_K        
    ,GET_CH1_B         
    ,GET_CH2_K          
    ,GET_CH2_B          
    ,GET_CH3_K
    ,GET_CH3_B
    ,GET_CH1_DUTY     
    ,GET_CH2_DUTY       
    ,GET_CH3_DUTY    
    ,DEVICE_SELF_TEST
    ,GET_TRANSMITTER_DATA_DIRECT
    ,GET_PROBE_DATA_DIRECT
    ,SET_CALIB_POINT
    ,GET_ALL_PARA
    ,GET_KALMAN_EN
};

const unsigned char FACTORY_SET_CMD[] = {
    0
    ,SET_PRODUCT_TYPE
    ,SET_PROBE_SN
    ,SET_TRANSMITTER_TYPE
    ,SET_TRANSMITTER_SN
    ,SET_RH_COEF
    ,SET_T_COEF
    ,SET_TD_COEF
    ,SET_TF_COEF
    ,SET_TW_COEF
    ,SET_E_COEF
    ,SET_R_COEF
    ,SET_DV_COEF
    ,SET_H_COEF
    ,SET_PWM1_COEF
    ,SET_PWM2_COEF
    ,SET_PWM3_COEF
    ,SET_PWM_DUTY_OUT
    ,RESET_TRANSMITTER
    ,RESET_PROBE
    ,VOLTAGE_GENERATE
    ,CLEAR_CALIB_STATUS
    ,UPDATE_TRANSMITTER_FIRMWARE_D
    ,UPDATE_TRANSMITTER_FIRMWARE
    ,UPDATE_PROBE_FIRMWARE_D
    ,UPDATE_PROBE_FIRMWARE
    ,SET_KALMAN_EN
};

CMD_DICT_ENTRY pc2board_cmd_dict_run(unsigned char cmd_desc)
{
	unsigned char low = 0;
	unsigned char high = sizeof(pc2board_cmd_list) / sizeof(CMD_DICT_ENTRY);
	unsigned char probe = 0;

	while((high - low) > 1) {
		probe = (high + low) / 2;

		if(pc2board_cmd_list[probe].cmd_key > cmd_desc) {
			high = probe;
		} else {
			low = probe;
		}
	}
    CMD_DICT_ENTRY  e;
	if((pc2board_cmd_list[low].cmd_key != cmd_desc)) {
		e.pFunc = NULL;
        e.cmd_key = cmd_desc;
	}
    e = pc2board_cmd_list[low];
	return e;
}


unsigned char get_cmd_desc(unsigned char uFunCode,unsigned char offset)
{
    unsigned char cmd_desc = GET_T;
    switch(uFunCode)
    {
    case 0x03:
      cmd_desc = USER_GET_CMD[offset];
      break;
    case 0x10:
      cmd_desc = USER_SET_CMD[offset];
      break;
    case 0x21:
      cmd_desc = FACTORY_GET_CMD[offset];
      break;
    case 0x22:
      cmd_desc = FACTORY_SET_CMD[offset];
      break;
    default:
      break;
    };
    return cmd_desc;
}
