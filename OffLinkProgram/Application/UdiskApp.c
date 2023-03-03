/********************************** (C) COPYRIGHT *******************************
 * File Name          : SPI_FLAH.c
 * Author             : WCH
 * Version            : V1.0.1
 * Date               : 2022/11/24
 * Description        : SPI FLASHChip operation file
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/******************************************************************************/
/* Header Files */
#include <Udisk.h>
#include "drv_spi.h"
#include "25qxx.h"
#include "drv_log.h"
#include "usb_lib.h"

const static char *TAG = "Udisk";
/*******************************************************************************
* Function Name  : FLASH_Port_Init
* Description    : FLASH chip operation related pins and hardware initialization
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
rt_device_t device;

static struct spi_configure config = {
    .SPI_Clock = SPI_Clock_36M,
    .SPI_DataSize = SPI_DataSize_8,
    .SPI_Direction = SPI_2Lines_FullDuplex,
    .SPI_FirstBit = SPI_First_MSB,
    .SPI_Mode = SPI_MODE_3,
    .cs = -1,
};

void _25qxx_spi_cs(uint8_t value)
{
    rt_pin_write(4, value);
}

void _25qxx_spi_rw(uint8_t *data,uint32_t len)
{
    rt_device_read(device,0x00,data,len);
}

struct _25qxx_function fun = {
    ._25qxx_spi_cs = _25qxx_spi_cs,
    ._25qxx_spi_rw = _25qxx_spi_rw,
};



void Udisk_Usb_Send(uint8_t *data,uint32_t len)
{
    USB_SIL_Write( EP4_IN, data, len );
}

void Uidsk_Tx_Stall(void)
{
    SetEPTxStatus(ENDP4, EP_TX_STALL);
}

void Uidsk_Rx_Stall(void)
{
    SetEPTxStatus(ENDP5, EP_RX_STALL);
}



static struct udisk_function udisk_fun = {
   .Udisk_Sector_size = 4096,
   .Udisk_Erase_Sector = _25qxx_Flash_Erase_Sector,
   .Udisk_Read_Data =  _25qxx_Flash_Read_Data,
   .Udisk_Write_Data = _25qxx_Flash_Write_NoCheck,
   .Udisk_Usb_Send = Udisk_Usb_Send,
   .Udisk_Set_Usb_Rx_Stall = Uidsk_Rx_Stall,
   .Udisk_Set_Usb_Tx_Stall = Uidsk_Tx_Stall,
};

int FLASH_Port_Init( void )
{
    rt_pin_mode(4, PIN_MODE_OUTPUT);
    device = rt_device_find("spi1");
    rt_device_control(device, RT_DEVICE_CTRL_CONFIG, &config);
    rt_device_init(device);
    rt_device_open(device,RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_DMA_RX | RT_DEVICE_FLAG_DMA_TX);
//    rt_device_open(device,RT_DEVICE_FLAG_RDWR);
    _25qxx_Init(&fun);
    /* Enable Udisk */
    udisk_fun.Sector_Number = _25qxx_Flash_Sector_Count();
    udisk_fun.Flash_ID = _25qxx_ReadID();;
    Udisk_Init(&udisk_fun);
    RT_LOGI(TAG,"Usb Udisk Init");
    return 0;
}
INIT_COMPONENT_EXPORT(FLASH_Port_Init);
