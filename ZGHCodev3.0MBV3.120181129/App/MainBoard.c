#include "common.h"
#include "include.h"
#include "MainBoard.h"
#include "MK60_flash.h"

const double KALMAN_PARA_ADC_Q		= 0.000001;
const double KALMAN_PARA_ADC_R		= 0.1;

const double KALMAN_PARA_REFPLS_Q	= 0.000001;
const double KALMAN_PARA_REFPLS_R	= 0.1;

const double KALMAN_PARA_MEAPLS_Q	= 0.000001;
const double KALMAN_PARA_MEAPLS_R	= 0.1;

//出厂设置内容
extern uint16 FACTORY_LIST_RHC[101]	= {26380,26453,26525,26595,26664,26731,26798,26863,26927,26989,27050,27111,27170,27227,27284,27340,27395,27448,27501,27552,27603,27653,27702,27750,27797,27843,27889,27934,27978,28021,28064,28106,28148,28189,28229,28269,28308,28347,28386,28424,28462,28500,28537,28574,28611,28648,28684,28721,28757,28793,28830,28866,28903,28939,28976,29013,29051,29088,29126,29164,29203,29242,29281,29321,29362,29403,29445,29487,29530,29574,29619,29665,29711,29759,29807,29857,29907,29959,30012,30066,30121,30178,30236,30295,30356,30418,30482,30548,30615,30683,30754,30826,30900,30976,31054,31134,31216,31300,31386,31474,31565};

extern float adcValue 			= 0.0f;
extern float adcRefValue		= 0.0f;
extern float pTomValue			= 0.0f;
extern uint32 refPlsCnt			= 0;
extern uint32 meaPlsCnt			= 0;
extern uint16 meaTRatio			= 0;

static const float B1 			= 0.0011;
static const float B2 			= 0.0892;
static const float B3 			= -0.0268;
static const float B4 			= -2.079;

/*
deltaRH = (B1 * RHldy + B2) * T + (B3 * RHldy + B4)

B1 = 0.0014;
B2 = 0.1325;
B3 = -0.0317;
B4 = -3.0876;

RHadj = RHldy + deltaRH

//this function build a array
meaT*100/refT = p1*RHadj4 + p2*RHadj3 + p3*RHadj2 + p4*RHadj + p5 
//

p1 =   -5.41e-07;
p2 =   0.0001392;
p3 =    -0.01295;
p4 =      0.7399;
p5 =         170;

RHadj - RHldy = (B1 * RHldy + B2) * T + (B3 * RHldy + B4)
--->
RHldy = (RHadj - B2*T - B4)/(B1*T + B3 + 1)
*/

extern uint16 FACTORY_LIST_TIOUT_420_MA[21]	= {\
646,/*0*/ \
646,/*1*/ \
646,/*2*/ \
646,/*3*/ \
521,/*4*/ \
700,/*5*/ \
818,/*6*/ \
967,/*7*/ \
1116,/*8*/ \
1264,/*9*/ \
1413,/*10*/ \
1562,/*11*/ \
1711,/*12*/ \
1859,/*13*/ \
2008,/*14*/ \
2157,/*15*/ \
2305,/*16*/ \
2454,/*17*/ \
2603,/*18*/ \
2751,/*19*/ \
2900,/*20*/ \
};

extern uint16 FACTORY_LIST_RHIOUT_420_MA[21]	= {\
651,/*0*/ \
651,/*1*/ \
651,/*2*/ \
651,/*3*/ \
519,/*4*/ \
668,/*5*/ \
817,/*6*/ \
965,/*7*/ \
1114,/*8*/ \
1263,/*9*/ \
1412,/*10*/ \
1561,/*11*/ \
1710,/*12*/ \
1858,/*13*/ \
2007,/*14*/ \
2156,/*15*/ \
2305,/*16*/ \
2454,/*17*/ \
2602,/*18*/ \
2751,/*19*/ \
2900,/*20*/ \
};

/******************
input :adcValue 16bit uint16
output:tempValue float
*******************/

float TempNowCalculate(float voltmv6port) 
{
    //uint16 ResValue = 0;
    float TempValue = 0.0;

    if(voltmv6port > 3000.0)
	TempValue = RANGE_PT_T_4;
    else
    	TempValue = (GH_MB_FlashData_t.F_T_P1_COFF)*voltmv6port + GH_MB_FlashData_t.F_T_P2_COFF;//LDY_T = p1 * POM + p2
    
    if(TempValue < RANGE_PT_T_4)
	TempValue = RANGE_PT_T_4;    
    
    if(TempValue > RANGE_PT_T_20)
	TempValue = RANGE_PT_T_20;
    
    return TempValue;
}

/************
input :uint16 meaTRatio	(eg: 178.96 --> 17896)
output:float rhadj	(eg: 23.13  --> 23.13)
*************/
float RHadjNowCalculate(uint16 meaTRatio)
{
    float rhadj = 0.0;
    unsigned char startRH = 0,endRH = 100,midRH = 50;
    
    //
    while(1)
    {
	if((GH_MB_FlashData_t.F_LIST_RHC)[midRH] < meaTRatio)
	    startRH = midRH;
	else
	    if((GH_MB_FlashData_t.F_LIST_RHC)[midRH] > meaTRatio)
		endRH = midRH;
	    else
		break;
	    
	if(endRH - startRH == 1)
	{ 
	    midRH = startRH;
	    break;
	}
	else
	    midRH = (int)((startRH + endRH)*1.0/2);
	
    }
    //end

    rhadj = ((meaTRatio - (GH_MB_FlashData_t.F_LIST_RHC)[midRH])*1.0/((GH_MB_FlashData_t.F_LIST_RHC)[midRH + 1] - (GH_MB_FlashData_t.F_LIST_RHC)[midRH]) + midRH)*1.0;
    

    
    return rhadj;
}

/******************
input1:float tempValue ,input2: float rhadj
output:float rhldy
*******************/

float RHldyNowCalculate(float tempValue,float rhadj) 
{
    float rhldy = 0.0;
    

    rhldy = rhadj + (B1 * rhadj + B2) * tempValue + (B3 * rhadj + B4);


    return rhldy;
}

uint16 RHDutyCalculate(float rhldy)
{
    float Iout = 0.0;
    uint16 duty = 0;
    uint16 Itemp = 0;
    
    if(rhldy < RANGE_RH_RATIO_4)
	rhldy = RANGE_RH_RATIO_4;    
    
    if(rhldy > RANGE_RH_RATIO_20)
	rhldy = RANGE_RH_RATIO_20;
    
    Iout = ((rhldy - GH_MB_FlashData_t.F_RH_RANGEL) * 1.0 / (GH_MB_FlashData_t.F_RH_RANGEH - GH_MB_FlashData_t.F_RH_RANGEL)) * 16.0 + 4.0;

    Itemp = (uint16)Iout;
    
    if(Itemp < 4)
	return (GH_MB_FlashData_t.F_LIST_RHIOUT_420_MA)[4];
    else
      	if(Itemp >= 20)
	    return (GH_MB_FlashData_t.F_LIST_RHIOUT_420_MA)[20];
      	else
      	{
	    duty = (uint16)((Iout - Itemp)*((GH_MB_FlashData_t.F_LIST_RHIOUT_420_MA)[Itemp + 1] - (GH_MB_FlashData_t.F_LIST_RHIOUT_420_MA)[Itemp]) + (GH_MB_FlashData_t.F_LIST_RHIOUT_420_MA)[Itemp] + 0.5);
	    return duty;
      	}
}

//0℃ : 4mA	200℃ : 20mA
uint16 TDutyCalculate(float tempValue)
{
    float Iout = 0.0;

    uint16 duty = 0;
    unsigned char Itemp = 0;
    
    //CALC Iout VALUE FROM T VALUE
    Iout = ((tempValue - GH_MB_FlashData_t.F_T_RANGEL) * 1.0 / (GH_MB_FlashData_t.F_T_RANGEH - GH_MB_FlashData_t.F_T_RANGEL)) * 16.0 + 4.0;

    Itemp = (uint16)Iout;
    
    if(Itemp < 4)
	return (GH_MB_FlashData_t.F_LIST_TIOUT_420_MA)[4];
    else
      	if(Itemp >= 20)
	    return (GH_MB_FlashData_t.F_LIST_TIOUT_420_MA)[20];
      	else
      	{
	    duty = (uint16)((Iout - Itemp)*((GH_MB_FlashData_t.F_LIST_TIOUT_420_MA)[Itemp + 1] - (GH_MB_FlashData_t.F_LIST_TIOUT_420_MA)[Itemp]) + (GH_MB_FlashData_t.F_LIST_TIOUT_420_MA)[Itemp] + 0.5);
	    return duty;
      	}
}

float SqrtForRes(float res)
{
    float result = 0.0;
    
    arm_sqrt_f32(res,&result);
    return result;
}



float KalmanFilterADC(unsigned long xnow)
{
    static float Ky_pre = 1100.0;
    static float Kp_offset_pre = 2;
    
    float Kg = 0.0;
    float Ky_now = 0.0;
    float Kp_offset_now = 0.0;
    float Kp_temp = 0.0;
    
    Kp_temp = Kp_offset_pre + KALMAN_PARA_ADC_Q;
    Kg = Kp_temp*1.0/(Kp_temp + KALMAN_PARA_ADC_R);
    Kp_offset_now = (1 - Kg)*Kp_temp;
    Ky_now = Ky_pre + (xnow - Ky_pre)*Kg;
    
    Ky_pre = Ky_now;
    Kp_offset_pre = Kp_offset_now;
    
    return Ky_now;
}

float KalmanFilterADCREF(unsigned long xnow)
{
    static float Ky_pre = 5000.0;
    static float Kp_offset_pre = 2;
    
    float Kg = 0.0;
    float Ky_now = 0.0;
    float Kp_offset_now = 0.0;
    float Kp_temp = 0.0;
    
    Kp_temp = Kp_offset_pre + KALMAN_PARA_ADC_Q;
    Kg = Kp_temp*1.0/(Kp_temp + KALMAN_PARA_ADC_R);
    Kp_offset_now = (1 - Kg)*Kp_temp;
    Ky_now = Ky_pre + (xnow - Ky_pre)*Kg;
    
    Ky_pre = Ky_now;
    Kp_offset_pre = Kp_offset_now;
    
    return Ky_now;
}

float KalmanFilterREFPLS(unsigned long xnow)
{
    static float Ky_pre = 5800.0;
    static float Kp_offset_pre = 2;
    
    float Kg = 0.0;
    float Ky_now = 0.0;
    float Kp_offset_now = 0.0;
    float Kp_temp = 0.0;
    
    Kp_temp = Kp_offset_pre + KALMAN_PARA_REFPLS_Q;
    Kg = Kp_temp*1.0/(Kp_temp + KALMAN_PARA_REFPLS_R);
    Kp_offset_now = (1 - Kg)*Kp_temp;
    Ky_now = Ky_pre + (xnow - Ky_pre)*Kg;
    
    Ky_pre = Ky_now;
    Kp_offset_pre = Kp_offset_now;
    
    return Ky_now;
}

float KalmanFilterMEAPLS(unsigned long xnow)
{
    static float Ky_pre = 1100.0;
    static float Kp_offset_pre = 2;
    
    float Kg = 0.0;
    float Ky_now = 0.0;
    float Kp_offset_now = 0.0;
    float Kp_temp = 0.0;
    
    Kp_temp = Kp_offset_pre + KALMAN_PARA_MEAPLS_Q;
    Kg = Kp_temp*1.0/(Kp_temp + KALMAN_PARA_MEAPLS_R);
    Kp_offset_now = (1 - Kg)*Kp_temp;
    Ky_now = Ky_pre + (xnow - Ky_pre)*Kg;
    
    Ky_pre = Ky_now;
    Kp_offset_pre = Kp_offset_now;
    
    return Ky_now;
}

float KalmanFilterRH(float xnow)
{
    static int index = 0;
    
    static float Ky_pre = 1100.0;
    static float Kp_offset_pre = 2;
    
    float Kg = 0.0;
    float Ky_now = 0.0;
    float Kp_offset_now = 0.0;
    float Kp_temp = 0.0;
    
    if(index > 10)
    {
	Kp_temp = Kp_offset_pre + KALMAN_PARA_MEAPLS_Q;
	Kg = Kp_temp*1.0/(Kp_temp + KALMAN_PARA_MEAPLS_R);
	Kp_offset_now = (1 - Kg)*Kp_temp;
	Ky_now = Ky_pre + (xnow - Ky_pre)*Kg;

	Ky_pre = Ky_now;
	Kp_offset_pre = Kp_offset_now;

	return Ky_now;
    }
    else
    {
	index++;
	Ky_pre = xnow;
	return xnow;
    }
}