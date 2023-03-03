#include "drv_spi.h"
#include "drv_log.h"
#include "drivers/pin.h"
#include "drv_dma.h"


const static char *TAG = "SPI";

enum {
    SPI1_INDEX,
    SPI2_INDEX,
};

struct ch32_spi_hw_config ch32_spi_hw_config[] =
{
    {
        -1,       //PA4
        0x05,       //PA5
        0x06,       //PA6
        0x07,       //PA7
        RCC_APB2Periph_SPI1,
    },
    {
		-1,       //PB12
		0x1D,       //PB13
		0x1E,       //PB14
		0x1F,       //PB15
        RCC_APB1Periph_SPI2,
    },
};

struct ch32_spi_config ch32_spi_config[] =
{
    {
        "spi1",
        SPI1,
        SPI1_IRQn,
    },
    {
        "spi2",
        SPI2,
        SPI2_IRQn,
    },
};

struct ch32_spi ch32_spi[sizeof(ch32_spi_config)/sizeof(struct ch32_spi_config)] = {0};


rt_err_t drv_spi_init(rt_device_t dev)
{
    struct ch32_spi *p = (struct ch32_spi *)dev->user_data;

    if(p->config->Instance == SPI1)
        RCC_APB2PeriphClockCmd(p->hw_config->spi_periph_clock,ENABLE);
    else
        RCC_APB1PeriphClockCmd(p->hw_config->spi_periph_clock,ENABLE);

    if(p->spi_configure.cs != -1){
        p->hw_config->cs = p->spi_configure.cs;
        rt_pin_mode(p->hw_config->cs,PIN_MODE_OUTPUT);
        rt_pin_write(p->hw_config->cs, 1);
    }
    
    rt_pin_mode(p->hw_config->sclk,PIN_MODE_AF_PP);
    rt_pin_mode(p->hw_config->miso,PIN_MODE_INPUT);
    rt_pin_mode(p->hw_config->mosi,PIN_MODE_AF_PP);

    RT_LOGI(TAG,"SPI%d Gpio Init CS:%d SCLK:%d MISO:%d MOSI:%d",dev->device_id,
                                                                p->hw_config->cs,
                                                                p->hw_config->sclk,
                                                                p->hw_config->miso,
                                                                p->hw_config->mosi);
    p->Init.SPI_Mode = SPI_Mode_Master;
    p->Init.SPI_NSS = SPI_NSS_Soft;
    p->Init.SPI_CRCPolynomial = 7;
    switch(p->spi_configure.SPI_Clock)
    {
        case SPI_Clock_0M5625:
            p->Init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
        break;

        case SPI_Clock_1M125:
            p->Init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
        break;

        case SPI_Clock_2M25:
            p->Init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
        break;

        case SPI_Clock_4M5:
            p->Init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
        break;

        case SPI_Clock_9M:
            p->Init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
        break;

        case SPI_Clock_18M:
            p->Init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
        break;     

        case SPI_Clock_36M:
            p->Init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
        break;      

        case SPI_Clock_72M:
            p->Init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
        break;   
    }

    switch(p->spi_configure.SPI_DataSize)
    {
        case SPI_DataSize_16:
            p->Init.SPI_DataSize = SPI_DataSize_16b;
        break;

        case SPI_DataSize_8:
            p->Init.SPI_DataSize = SPI_DataSize_8b;
        break;
    }

    switch(p->spi_configure.SPI_Direction)
    {
        case SPI_2Lines_FullDuplex:
            p->Init.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
        break;

        case SPI_2Lines_RxOnly:
            p->Init.SPI_Direction = SPI_Direction_2Lines_RxOnly;
        break;

        case SPI_1Line_Rx:
            p->Init.SPI_Direction = SPI_Direction_1Line_Rx;
        break;

        case SPI_1Line_Tx:
            p->Init.SPI_Direction = SPI_Direction_1Line_Tx;
        break;
    }

    switch (p->spi_configure.SPI_FirstBit)
    {
        case SPI_First_MSB:
            p->Init.SPI_FirstBit = SPI_FirstBit_MSB;
        break;
        
        case SPI_First_LSB:
            p->Init.SPI_FirstBit = SPI_FirstBit_LSB;
        break;
    }

    switch (p->spi_configure.SPI_Mode)
    {
        case SPI_MODE_0:
            p->Init.SPI_CPOL = SPI_CPOL_Low;
            p->Init.SPI_CPHA = SPI_CPHA_1Edge;
        break;

        case SPI_MODE_1:
            p->Init.SPI_CPOL = SPI_CPOL_Low;
            p->Init.SPI_CPHA = SPI_CPHA_2Edge;
        break;    

        case SPI_MODE_2:
            p->Init.SPI_CPOL = SPI_CPOL_High;
            p->Init.SPI_CPHA = SPI_CPHA_1Edge;
        break;    

        case SPI_MODE_3:
            p->Init.SPI_CPOL = SPI_CPOL_High;
            p->Init.SPI_CPHA = SPI_CPHA_2Edge;
        break;            
    }

    SPI_Init(p->config->Instance,&p->Init);

    return RT_EOK;
}

rt_err_t drv_spi_open(rt_device_t dev, rt_uint16_t oflag)
{
    struct ch32_spi *p = (struct ch32_spi *)dev->user_data;
    dev->open_flag = oflag;
    if(dev->open_flag & RT_DEVICE_FLAG_DMA_TX){
        DMA1_Channel_Init(DMA1_Channel3);
        SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
    }

    if(dev->open_flag & RT_DEVICE_FLAG_DMA_RX){
        DMA1_Channel_Init(DMA1_Channel2);
        SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);
    }

    SPI_Cmd(p->config->Instance,ENABLE);
    RT_LOGI(TAG,"SPI%d Open",dev->device_id);
    return RT_EOK;
}

rt_err_t drv_spi_close(rt_device_t dev)
{
    struct ch32_spi *p = (struct ch32_spi *)dev->user_data;
    SPI_Cmd(p->config->Instance,DISABLE);
    return RT_EOK;
}

void _spi_cs_control(rt_device_t dev,rt_base_t value)
{
    struct ch32_spi *p = (struct ch32_spi *)dev->user_data;
    if(p->hw_config->cs == -1) return;
    rt_pin_write(p->hw_config->cs, value);
}

uint8_t _spi_poll_txrx(rt_device_t dev, rt_uint8_t *data,int length)
{
    struct ch32_spi *p = (struct ch32_spi *)dev->user_data;
    for(int i = 0;i< length;i++){
        while(SPI_I2S_GetFlagStatus(p->config->Instance, SPI_I2S_FLAG_TXE) == RESET);
        SPI_I2S_SendData(p->config->Instance, data[i]);
        while(SPI_I2S_GetFlagStatus(p->config->Instance, SPI_I2S_FLAG_RXNE) == RESET);
        data[i] = SPI_I2S_ReceiveData(p->config->Instance);
    }
    return length;
}

void _spi_dma_tx(DMA_Channel_TypeDef *DMA_CHx, u32 ppadr, u32 memadr, u16 bufsize)
{
    DMA_InitTypeDef DMA_InitStructure = {0};

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_DeInit(DMA_CHx);

    DMA_InitStructure.DMA_PeripheralBaseAddr = ppadr;
    DMA_InitStructure.DMA_MemoryBaseAddr = memadr;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = bufsize;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA_CHx, &DMA_InitStructure);
}

void _spi_dma_rx(DMA_Channel_TypeDef *DMA_CHx, u32 ppadr, u32 memadr, u16 bufsize)
{
    DMA_InitTypeDef DMA_InitStructure = {0};

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_DeInit(DMA_CHx);

    DMA_InitStructure.DMA_PeripheralBaseAddr = ppadr;
    DMA_InitStructure.DMA_MemoryBaseAddr = memadr;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = bufsize;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA_CHx, &DMA_InitStructure);
}

int _spi_int_txrx(rt_device_t dev, rt_uint8_t *data, int length)
{

    return length;
}

int _spi_dma_txrx(rt_device_t dev, rt_uint8_t *data, int length)
{

    return length;
}


rt_size_t drv_spi_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    struct ch32_spi *p = (struct ch32_spi *)dev->user_data;
    rt_size_t len = 0;
    if(dev->open_flag & RT_DEVICE_FLAG_DMA_RX)
    {
        _spi_cs_control(dev, 0);
        _spi_dma_tx(DMA1_Channel3, (uint32_t)&p->config->Instance->DATAR, (uint32_t)buffer, size);
        _spi_dma_rx(DMA1_Channel2, (uint32_t)&p->config->Instance->DATAR, (uint32_t)buffer, size);
        DMA1_Channel_Write(DMA1_Channel3,DMA1_Channel2);
        _spi_cs_control(dev, 1);
    }
    else {
        _spi_cs_control(dev, 0);
        len = _spi_poll_txrx(dev,buffer,size);
        _spi_cs_control(dev, 1);
    }

    return len;
}

rt_size_t drv_spi_write(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size)
{
    struct ch32_spi *p = (struct ch32_spi *)dev->user_data;
    rt_size_t len = 0;
    if(dev->open_flag & RT_DEVICE_FLAG_DMA_TX)
    {
        _spi_cs_control(dev, 0);
        _spi_dma_tx(DMA1_Channel3, (uint32_t)&p->config->Instance->DATAR, (uint32_t)buffer, size);
        _spi_dma_rx(DMA1_Channel2, (uint32_t)&p->config->Instance->DATAR, (uint32_t)buffer, size);
        DMA1_Channel_Write(DMA1_Channel3,DMA1_Channel2);
        _spi_cs_control(dev, 1);
        len = size;
    }
    else {
        _spi_cs_control(dev, 0);
        len = _spi_poll_txrx(dev,(void *)buffer,size);
        _spi_cs_control(dev, 1);
    }
    return len;
}

rt_err_t drv_spi_control(rt_device_t dev, int cmd, void *args)
{
    struct ch32_spi *p = (struct ch32_spi *)dev->user_data;
    switch(cmd)
    {
        case RT_DEVICE_CTRL_CONFIG:
            rt_memcpy(&p->spi_configure,args,sizeof(struct spi_configure));
        break;
    }
    return RT_EOK;
}


int rt_hw_spi_init(void)
{
    struct spi_configure spi_configure = RT_SPI_CONFIG_DEFAULT;
    uint8_t num = sizeof(ch32_spi_config)/sizeof(struct ch32_spi_config);
    for(int i = 0; i < num; i++)
    {
        ch32_spi[i].hw_config = &ch32_spi_hw_config[i];
        ch32_spi[i].config = &ch32_spi_config[i];
        ch32_spi[i].spi_configure = spi_configure;
        ch32_spi[i].device.device_id = i + 1;
        ch32_spi[i].device.init = drv_spi_init;
        ch32_spi[i].device.open = drv_spi_open;
        ch32_spi[i].device.close = drv_spi_close;
        ch32_spi[i].device.read = drv_spi_read;
        ch32_spi[i].device.write = drv_spi_write;
        ch32_spi[i].device.control = drv_spi_control;
        ch32_spi[i].device.user_data = &ch32_spi[i];
        rt_device_register(&ch32_spi[i].device, ch32_spi[i].config->name, RT_DEVICE_FLAG_RDWR | 
                                                                            RT_DEVICE_FLAG_INT_RX | 
                                                                            RT_DEVICE_FLAG_DMA_RX |
                                                                            RT_DEVICE_FLAG_INT_TX |
                                                                            RT_DEVICE_FLAG_DMA_TX);
    }
    
    RT_LOGI(TAG,"RT SPI Init");
    return RT_EOK;
}
INIT_DEVICE_EXPORT(rt_hw_spi_init);
