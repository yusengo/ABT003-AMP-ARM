*****************20181201****************
1. ��V4�忨���У����ABT APP��Modbus����
*****************20181127****************
1. ��������ZGHv4�忨�����У��޸Ĳ���Ӳ����Ϣ��
��Ҫ��Ϣ���꣺
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
�Ķ���Ϣ���£�
	G1.	IOCTRL1:	PTA27 -> PTE24
		IOCTRL2:	PTA28 -> PTE25
	G2.	LED0:	PTA11 -> PTC4
	G3.	+LED1:	PTB9
		+KeyTest:	PTB7
2. ��AD7705��������
	G4.	+AD_\C\S:	PTE0	SPI1_PCS1	out
		+AD_DIN:		PTE1	SPI1_SOUT	out
		+AD_SCLK:	PTE2	SPI1_SCK		out
		+AD_DOUT:	PTE3	SPI1_SIN		in
		+AD_\R\S\T:	PTE4	--		out
		+AD_\D\R\D\Y	PTE5	--		in

*****************20180903****************
1. ModBusȫ����ֲ�ɹ����ɶ���д
2. �ο�����THP310�е�ModBus��������ֲ��K60���ʿɼ���ԭ���󲿷�ָ��
3. ��GH310��THP310�нϴ���죬��Щָ����¶��壬�������Ŀ¼�µ�excel
4. ��ModBus Poll��λ������115200�����ʣ��������Զ�д30000+�Σ������