#ifndef __DRV_SPI_H
#define __DRV_SPI_H

#include <rtthread.h>
#include "rtdevice.h"
#include "ch32v20x.h"



typedef enum SPI_Direction {
    SPI_2Lines_FullDuplex = 0,
    SPI_2Lines_RxOnly,
    SPI_1Line_Rx,
    SPI_1Line_Tx,
}SPI_Direction_t;


typedef enum SPI_DataSize {
    SPI_DataSize_16 = 0,
    SPI_DataSize_8,
}SPI_DataSize_t;

typedef enum SPI_Mode{
    SPI_MODE_0 = 0,     //CPOL:0 CPHA:0
    SPI_MODE_1,         //CPOL:0 CPHA:1
    SPI_MODE_2,         //CPOL:1 CPHA:0
    SPI_MODE_3,         //CPOL:1 CPHA:1
}SPI_Mode_t;

typedef enum SPI_Clock{
    SPI_Clock_0M5625 = 0,
    SPI_Clock_1M125,
    SPI_Clock_2M25,
    SPI_Clock_4M5,
    SPI_Clock_9M,
    SPI_Clock_18M,
    SPI_Clock_36M,
    SPI_Clock_72M,
}SPI_Clock_t;

typedef enum SPI_FirstBit{
    SPI_First_MSB = 0,
    SPI_First_LSB,
}SPI_FirstBit_t;

#define RT_SPI_CONFIG_DEFAULT           \
{                                          \
    -1,                         /* hw cs */  \
    SPI_2Lines_FullDuplex,      /* 2Lines FullDuplex */     \
    SPI_DataSize_8,             /* 8 databits  */      \
    SPI_MODE_0,                 /* CPOL:1 CPHA:1  */     \
    SPI_Clock_36M,              /* 72M bits/s */ \
    SPI_First_MSB,              /* Normal mode */    \
}

struct spi_configure
{
    int16_t cs;                         //If the value is - 1, use hardware slice selection
    SPI_Direction_t SPI_Direction;
    SPI_DataSize_t SPI_DataSize;
    SPI_Mode_t SPI_Mode;
    SPI_Clock_t SPI_Clock;
    SPI_FirstBit_t SPI_FirstBit;
};

struct ch32_spi_hw_config{
    int16_t cs;
    uint16_t sclk;
    uint16_t miso;
    uint16_t mosi;
    rt_uint32_t spi_periph_clock;
};

struct ch32_spi_config
{
    const char *name;
    SPI_TypeDef *Instance;
    IRQn_Type irq_type;
};

struct ch32_spi
{
    struct ch32_spi_hw_config *hw_config;
    struct ch32_spi_config *config;
    struct spi_configure spi_configure;
    SPI_InitTypeDef Init;
    struct rt_device device;
};

#endif
