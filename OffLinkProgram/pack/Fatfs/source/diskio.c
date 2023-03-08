/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "25qxx.h"
#include "drv_log.h"
/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
    return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{

    return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{

    for(int i = 0;i<count;i++){
        _25qxx_Flash_Read_Data(sector * SPI_FLASH_SectorSize,(uint8_t *)(buff + i*SPI_FLASH_SectorSize), SPI_FLASH_SectorSize);
    }

	 return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{

    for(int i = 0;i<count;i++){
        _25qxx_Flash_Write_Data(sector * SPI_FLASH_SectorSize, (uint8_t *)(buff + i*SPI_FLASH_SectorSize),SPI_FLASH_SectorSize);
    }
	 return RES_OK;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
    uint16_t value = 0xFFFF;

    switch(cmd)
    {
        case CTRL_SYNC:
            break;

        case CTRL_TRIM:
            break;
        case GET_BLOCK_SIZE:
            *(DWORD*)buff = value;
            break;
        case GET_SECTOR_SIZE:
            *(DWORD*)buff = SPI_FLASH_SectorSize;
            break;
        case GET_SECTOR_COUNT:
            *(DWORD*)buff = 2048;
            break;
        default:
            return RES_PARERR;
        break;
    }

	return RES_OK;
}

