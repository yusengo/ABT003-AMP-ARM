/*
---------炬远智能科技（上海）有限公司---------
---------版权所有 翻版必究-------------------
---------2018.07.18 yusengo@163.com---------
*/

#ifndef     _MK60_DAC_H_
#define     _MK60_DAC_H_ 1u

typedef enum DACn       //DAC模块
{
    DAC0,
    DAC1
} DACn_e;

extern void dac_init(DACn_e);               //DAC一次转换初始化
extern void dac_out(DACn_e, uint16 val);    //DAC一次转换操作

#endif  //_MK60_DAC_H_

