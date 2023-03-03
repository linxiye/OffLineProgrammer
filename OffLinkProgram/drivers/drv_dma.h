#ifndef __DRV_DMA_H
#define __DRV_DMA_H


#include "rtthread.h"
#include "ch32v20x.h"
#include <rtdevice.h>

struct ch32_dma_config{
    DMA_Channel_TypeDef *Instance;
    IRQn_Type irq_type;
};

void DMA1_Channel_Write(DMA_Channel_TypeDef *Tx, DMA_Channel_TypeDef *Rx);
void DMA1_Channel_Init(DMA_Channel_TypeDef *Instance);

#endif

