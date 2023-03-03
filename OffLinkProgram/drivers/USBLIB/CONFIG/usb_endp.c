/********************************** (C) COPYRIGHT *******************************
 * File Name          : usb_endp.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2021/08/08
 * Description        : Endpoint routines
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/ 
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_mem.h"
#include "hw_config.h"
#include "usb_istr.h"
#include "usb_pwr.h"
#include "usb_prop.h"
uint8_t USBD_Endp3_Busy;
uint8_t USBD_Endp4_Busy;
uint8_t USBD_Endp5_Busy;
uint16_t USB_Rx_Cnt=0;
uint8_t USBD_Endp5_BUFF[64];
static struct ch32_usb *dev;
/*********************************************************************
 * @fn      EP2_IN_Callback
 *
 * @brief  Endpoint 1 IN.
 *
 * @return  none
 */
void EP1_IN_Callback (void)
{ 

}

/*********************************************************************
 * @fn      EP2_OUT_Callback
 *
 * @brief  Endpoint 2 OUT.
 *
 * @return  none
 */
void EP2_OUT_Callback (void)
{ 
	uint32_t len;
	uint8_t buff[64];
    len = GetEPRxCount( EP2_OUT & 0x7F );
    PMAToUserBufferCopy( &buff[0], GetEPRxAddr( EP2_OUT & 0x7F ), len );
    SetEPRxValid( ENDP2 );
    rt_kprintf("%s\r\n",buff);
}
/*********************************************************************
 * @fn      EP3_IN_Callback
 *
 * @brief  Endpoint 3 IN.
 *
 * @return  none
 */
void EP3_IN_Callback (void)
{ 
	USBD_Endp3_Busy = 0;
}

void EP4_IN_Callback (void)
{
    USBD_Endp4_Busy = 1;
    dev->Udisk_In_Callback(NULL,0);
}

void EP5_OUT_Callback (void)
{
    USBD_Endp5_Busy = 1;

    USB_Rx_Cnt = USB_SIL_Read(ENDP5, USBD_Endp5_BUFF);
    dev->Udisk_Out_Callback( NULL, 0 );

}
/*********************************************************************
 * @fn      USBD_ENDPx_DataUp
 *
 * @brief  USBD ENDPx DataUp Function
 * 
 * @param   endp - endpoint num.
 *          *pbuf - A pointer points to data.
 *          len - data length to transmit.
 * 
 * @return  data up status.
 */
uint8_t USBD_ENDPx_DataUp( uint8_t endp, uint8_t *pbuf, uint16_t len )
{
	if( endp == ENDP3 )
	{
		if (USBD_Endp3_Busy)
		{
			return USB_ERROR;
		}
		USB_SIL_Write( EP3_IN, pbuf, len );
		USBD_Endp3_Busy = 1;
	}
	else
	{
		return USB_ERROR;
	}
	return USB_SUCCESS;
}

void USBD_Callback_Init(struct ch32_usb *p)
{
    dev = p;
}
