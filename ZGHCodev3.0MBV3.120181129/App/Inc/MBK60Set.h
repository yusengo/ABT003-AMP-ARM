#include "common.h"
#include "include.h"
#include "MK60_uart.h"

extern char rxDataFirst;
extern char rxData;

#define MBDEBUG 0

//#define MB_DIR_PORT PTB23 //2.0BOARD
#define MB_DIR_PORT PTB19 //3.0BOARD

#define MB_DIR_RX	0
#define MB_DIR_TX	1

#define MB_RXTX_PORT UART4

void ABT_MBUartRXIRQEN(void);

void ABT_MBUartRXIRQDIS(void);

void ABT_MBUartTXIRQEN(void);

void ABT_MBUartTXIRQDIS(void);


void ABT_MBTimer35TInit(uint32 timeOut);

void ABT_MBTimerEnable(void);

void ABT_MBTimerDisable(void);

void ABT_MBUartPutChar(char ch);

void ABT_MBUartGetChar(char *ch);

void ABT_MBUartInit(uint32 baud);
void ABT_MBInit(void);