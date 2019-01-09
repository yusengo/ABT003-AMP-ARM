
#include "stdlib.h"
#include "string.h"

#include "port.h"

#include "mb.h"
#include "mbframe.h"
#include "mbproto.h"
#include "mbconfig.h"
#include "MK60_flash.h"
#include "gemcreate_probe_cmd.h"

int updata_status = 0x5555AAAA;

eMBException
eMBVendorRead(UCHAR *pucFrame, USHORT *usLen) {
	//pucFrame: remove adress
	//usLen: actual data length,total - 3
	if ((NULL == pucFrame) || (NULL == usLen)) {
		return MB_EX_SLAVE_DEVICE_FAILURE;
	}
    
	USHORT ulen = pucFrame[4] << 1;
    if(ulen == 0)
    {
      ulen = 2;
    }
	pc2board_cmd_dict_run(get_cmd_desc(pucFrame[0], pucFrame[2])).pFunc(pucFrame, ulen);
	*usLen = 2 + ulen;
	return MB_EX_NONE;
}


eMBException
eMBVendorWrite(UCHAR *pucFrame, USHORT *usLen) {
	//pucFrame: remove adress
	//usLen: actual data length,total - 3
	if ((NULL == pucFrame) || (NULL == usLen)) {
		return MB_EX_SLAVE_DEVICE_FAILURE;
	}
	USHORT ulen = (pucFrame[3] << 8 | pucFrame[4]);
	pc2board_cmd_dict_run(get_cmd_desc(pucFrame[0], pucFrame[2])).pFunc(pucFrame, ulen*2);
    
	*usLen = 5;
	return MB_EX_NONE;
}



eMBException
eMBVendorFWrite(UCHAR *pucFrame, USHORT *usLen) {
	//pucFrame: remove adress
	//usLen: actual data length,total - 3
	if ((NULL == pucFrame) || (NULL == usLen)) {
		return MB_EX_SLAVE_DEVICE_FAILURE;
	}
	USHORT ulen = (pucFrame[3] << 8 | pucFrame[4]);
	pc2board_cmd_dict_run(get_cmd_desc(pucFrame[0], pucFrame[2])).pFunc(pucFrame, ulen);
    
	*usLen = 5;
	return MB_EX_NONE;
}

eMBException
eMBVendorIap(UCHAR *pucFrame, USHORT *usLen)
{
    return MB_EX_NONE;
}
/*
    unsigned int jump_flag = IAP_JUMP_CLEAR;
    if ((NULL == pucFrame) || (NULL == usLen)) {
		return MB_EX_SLAVE_DEVICE_FAILURE;
	}
    unsigned int is_in_iap = 0x5A5A5A5A;
    unsigned int iap_start_address_status = 0x00FF00FF;
    switch(pucFrame[2])
    {
    case 0x01:      //IS IN IAP
        memcpy(&pucFrame[2],&is_in_iap,4);
        pucFrame[1] = 4;        
        *usLen = 2 + 4;
        break;
    case 0x02:      //JUMP TO IAP
        write_flash(IAP_FLAG_ADDR,&jump_flag, 1);
        PBout(12) = 1;
        PBout(13) = 0;
        __set_FAULTMASK(1);
        NVIC_SystemReset();
        break;
    case 0x03:      //INIT IAP HEADER
#if 0
        memcpy(&iap_header,&pucFrame[5],sizeof(IAP_HEADER));
        if(iap_header.IAP_START_ADDR != IAP_APP_ADDR)
        {
            iap_start_address_status = 0x00FF00FF;
        }
        else 
        {
            iap_start_address_status = 0xFF00FF00;
        }
#endif
        iap_start_address_status = 0xFF00FF00;
        memcpy(&pucFrame[2],&iap_start_address_status,4);
        pucFrame[1] = 4;        
        *usLen = 2 + 4;
        break;
    case 0x04:      //RECEIVE IAP DATA PACKAGE
#if 0
        memcpy(&package_num,&pucFrame[5],4);
        memcpy(&iap_data[(package_num % (PAGE_SIZE / 128)) * 128],&pucFrame[9],128);
        if(package_num % (PAGE_SIZE / 128) == ((PAGE_SIZE / 128) - 1))
        {
            unsigned int adddd = iap_header.IAP_START_ADDR 
                                       +(package_num / (PAGE_SIZE / 128)) * 1024;
            
            updata_status = write_flash(
                                        adddd
                                       ,(unsigned int*) (iap_data)
                                       ,1024 / 4);
            
        }
#endif 
        memcpy(&pucFrame[2],&updata_status,4);
        pucFrame[1] = 4;        
        *usLen = 2 + 4;
        break;
    case 0x05:      //JUMP TO APP
#if 0
        set_jump_flag();
        jump_to_app();
#endif
        memcpy(&pucFrame[2],&is_in_iap,4);
        pucFrame[1] = 4;        
        *usLen = 2 + 4;
        break;
    }
    return MB_EX_NONE;
}
*/