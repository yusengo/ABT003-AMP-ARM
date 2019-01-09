/******************************************************************************
*            CopyRight(C), Shanghai JinKai Electronic Technology Co.,LTD.
* File Name: vendor_func.h
* Author: xujun
* Date: 2017-02-10
* Description: probe calib with freertos
* History:
* 1.Author: xujun
*   Date: 2017-02-10
*   Description: Initial
* 2.Author:
*   Date:
*   Description:
******************************************************************************/
#include "stdlib.h"
#include "string.h"

#include "port.h"

#include "mb.h"
#include "mbframe.h"
#include "mbproto.h"
#include "mbconfig.h"

#include "gemcreate_probe_cmd.h"


eMBException eMBVendorRead(UCHAR *pucFrame, USHORT *usLen);
eMBException eMBVendorWrite(UCHAR *pucFrame, USHORT *usLen);
eMBException eMBVendorFWrite(UCHAR *pucFrame, USHORT *usLen);
eMBException eMBVendorIap(UCHAR *pucFrame, USHORT *usLen);
