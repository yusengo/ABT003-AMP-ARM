*****************20181201****************
1. 在V4板卡运行，配合ABT APP可Modbus操作
*****************20181127****************
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

*****************20180903****************
1. ModBus全部移植成功，可读可写
2. 参考徐总THP310中的ModBus，重新移植到K60，故可兼容原来大部分指令
3. 因GH310与THP310有较大差异，有些指令被重新定义，具体见本目录下的excel
4. 用ModBus Poll上位机，以115200波特率，连续测试读写30000+次，零错误