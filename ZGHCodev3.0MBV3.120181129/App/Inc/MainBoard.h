#ifndef _MAINBOARD_H_
#define _MAINBOARD_H_    

#define ADJORDER_T		1

//单位℃
#define RANGE_PT_T_4		0.00f
#define RANGE_PT_T_20		200.00f

//单位Ω
#define RANGE_PT_R_4		1000
#define RANGE_PT_R_20		1800

//单位pF
#define RANGE_RH_C_4		100
#define RANGE_RH_C_20		200

//
//ratio
#define RANGE_RH_RATIO_4	0.00f
#define RANGE_RH_RATIO_20	100.00f


#define DUTY_010V_0V		10
#define DUTY_010V_10V		200


static const float B1;
static const float B2;
static const float B3;
static const float B4;

extern uint16 FACTORY_LIST_RHC[101];
extern uint16 FACTORY_LIST_TIOUT_420_MA[21];
extern uint16 FACTORY_LIST_RHIOUT_420_MA[21];

extern float adcValue;
extern float adcRefValue;
extern float pTomValue;
extern uint32 meaPlsCnt;
extern uint32 refPlsCnt;
extern uint16 meaTRatio;
    
float RHldyNowCalculate(float tempValue,float rhadj);
float RHadjNowCalculate(uint16 meaTRatio);

float TempNowCalculate(float adcValue);
float SqrtForRes(float res);
uint16 RHDutyCalculate(float rhldy);
uint16 TDutyCalculate(float tempValue);
float KalmanFilterADC(unsigned long xnow);
float KalmanFilterADCREF(unsigned long xnow);
float KalmanFilterMEAPLS(unsigned long xnow);
float KalmanFilterREFPLS(unsigned long xnow);
float KalmanFilterRH(float xnow);

#endif  //__INCLUDE_H__
