#include "drv_dma.h"
#include "drv_log.h"


void DMA1_Channel1_IRQHandler(void) __attribute__((interrupt()));
void DMA1_Channel2_IRQHandler(void) __attribute__((interrupt()));
void DMA1_Channel3_IRQHandler(void) __attribute__((interrupt()));
void DMA1_Channel4_IRQHandler(void) __attribute__((interrupt()));
void DMA1_Channel5_IRQHandler(void) __attribute__((interrupt()));
void DMA1_Channel6_IRQHandler(void) __attribute__((interrupt()));
void DMA1_Channel7_IRQHandler(void) __attribute__((interrupt()));
void DMA1_Channel8_IRQHandler(void) __attribute__((interrupt()));

struct DmaCH {
    uint8_t UserCh;
    uint8_t IrqCh;
    struct rt_completion completion;
};
static struct DmaCH DmaCH;
const static char *TAG = "DMA";

enum {
    DMA1_Channel1_INDEX,
    DMA1_Channel2_INDEX,
    DMA1_Channel3_INDEX,
    DMA1_Channel4_INDEX,
    DMA1_Channel5_INDEX,
    DMA1_Channel6_INDEX,
    DMA1_Channel7_INDEX,
    DMA1_Channel8_INDEX,
};


struct ch32_dma_config ch32_dma_config[8] = {
    {
        .Instance = DMA1_Channel1,
        .irq_type = DMA1_Channel1_IRQn,
    },
    {
        .Instance = DMA1_Channel2,
        .irq_type = DMA1_Channel2_IRQn,
    },
    {
        .Instance = DMA1_Channel3,
        .irq_type = DMA1_Channel3_IRQn,
    },
    {
        .Instance = DMA1_Channel4,
        .irq_type = DMA1_Channel4_IRQn,
    },
    {
        .Instance = DMA1_Channel5,
        .irq_type = DMA1_Channel5_IRQn,
    },
    {
        .Instance = DMA1_Channel6,
        .irq_type = DMA1_Channel6_IRQn,
    },
    {
        .Instance = DMA1_Channel7,
        .irq_type = DMA1_Channel7_IRQn,
    },
    {
        .Instance = DMA1_Channel8,
        .irq_type = DMA1_Channel8_IRQn,
    },
};

void DMA1_Channel1_IRQHandler(void)
{
    GET_INT_SP();
    rt_interrupt_enter();
    if(DMA_GetITStatus(DMA1_IT_TC1) == SET){
        DMA_ClearITPendingBit(DMA1_IT_TC1);
        DmaCH.IrqCh |= (1 << DMA1_Channel1_INDEX);
        if(DmaCH.IrqCh == DmaCH.UserCh)
            rt_completion_done(&(DmaCH.completion));
    }
    rt_interrupt_leave();
    FREE_INT_SP();
}

void DMA1_Channel2_IRQHandler(void)
{
    GET_INT_SP();
    rt_interrupt_enter();

    if(DMA_GetITStatus(DMA1_IT_TC2) == SET){
        DMA_ClearITPendingBit(DMA1_IT_TC2);
        DmaCH.IrqCh |= (1 << DMA1_Channel2_INDEX);
        if(DmaCH.IrqCh == DmaCH.UserCh)
            rt_completion_done(&(DmaCH.completion));
    }
    rt_interrupt_leave();
    FREE_INT_SP();
}

void DMA1_Channel3_IRQHandler(void)
{
    GET_INT_SP();
    rt_interrupt_enter();
    if(DMA_GetITStatus(DMA1_IT_TC3) == SET){
        DMA_ClearITPendingBit(DMA1_IT_TC3);
        DmaCH.IrqCh |= (1 << DMA1_Channel3_INDEX);
        if(DmaCH.IrqCh == DmaCH.UserCh)
            rt_completion_done(&(DmaCH.completion));
    }
    rt_interrupt_leave();
    FREE_INT_SP();
}

void DMA1_Channel4_IRQHandler(void)
{
    GET_INT_SP();
    rt_interrupt_enter();
    if(DMA_GetITStatus(DMA1_IT_TC4) == SET){
        DMA_ClearITPendingBit(DMA1_IT_TC4);
        DmaCH.IrqCh |= (1 << DMA1_Channel4_INDEX);
        if(DmaCH.IrqCh == DmaCH.UserCh)
            rt_completion_done(&(DmaCH.completion));
    }
    rt_interrupt_leave();
    FREE_INT_SP();
}

void DMA1_Channel5_IRQHandler(void)
{
    GET_INT_SP();
    rt_interrupt_enter();
    if(DMA_GetITStatus(DMA1_IT_TC5) == SET){
        DMA_ClearITPendingBit(DMA1_IT_TC5);
        DmaCH.IrqCh |= (1 << DMA1_Channel5_INDEX);
        if(DmaCH.IrqCh == DmaCH.UserCh)
            rt_completion_done(&(DmaCH.completion));
    }
    rt_interrupt_leave();
    FREE_INT_SP();
}

void DMA1_Channel6_IRQHandler(void)
{
    GET_INT_SP();
    rt_interrupt_enter();
    if(DMA_GetITStatus(DMA1_IT_TC6) == SET){
        DMA_ClearITPendingBit(DMA1_IT_TC6);
        DmaCH.IrqCh |= (1 << DMA1_Channel6_INDEX);
        if(DmaCH.IrqCh == DmaCH.UserCh)
            rt_completion_done(&(DmaCH.completion));
    }
    rt_interrupt_leave();
    FREE_INT_SP();
}

void DMA1_Channel7_IRQHandler(void)
{
    GET_INT_SP();
    rt_interrupt_enter();
    if(DMA_GetITStatus(DMA1_IT_TC7) == SET){
        DMA_ClearITPendingBit(DMA1_IT_TC7);
        DmaCH.IrqCh |= (1 << DMA1_Channel7_INDEX);
        if(DmaCH.IrqCh == DmaCH.UserCh)
            rt_completion_done(&(DmaCH.completion));
    }
    rt_interrupt_leave();
    FREE_INT_SP();
}

void DMA1_Channel8_IRQHandler(void)
{
    GET_INT_SP();
    rt_interrupt_enter();
    if(DMA_GetITStatus(DMA1_IT_TC8) == SET){
        DMA_ClearITPendingBit(DMA1_IT_TC8);
        DmaCH.IrqCh |= (1 << DMA1_Channel8_INDEX);
        if(DmaCH.IrqCh == DmaCH.UserCh)
            rt_completion_done(&(DmaCH.completion));
    }
    rt_interrupt_leave();
    FREE_INT_SP();
}

void DMA1_Channel_Init(DMA_Channel_TypeDef *Instance)
{
    NVIC_InitTypeDef NVIC_InitStruct = {0};

    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

    for(int i = 0; i < sizeof(ch32_dma_config)/sizeof(struct ch32_dma_config);i++)
    {
        if(ch32_dma_config[i].Instance == Instance){

            NVIC_InitStruct.NVIC_IRQChannel = ch32_dma_config[i].irq_type;
            NVIC_Init(&NVIC_InitStruct);
            break;
        }
    }

}

void DMA1_Channel_Write(DMA_Channel_TypeDef *Tx, DMA_Channel_TypeDef *Rx)
{
    uint8_t tx = 0,rx = 0;
    DmaCH.UserCh = 0;
    DmaCH.IrqCh = 0;
    for(int i = 0; i < sizeof(ch32_dma_config)/sizeof(struct ch32_dma_config);i++)
    {
        if(ch32_dma_config[i].Instance == Tx && Tx != NULL)
            tx = i;
        if(ch32_dma_config[i].Instance == Rx && Rx != NULL)
            rx = i;
    }
    if(Tx != NULL)
        DMA_ITConfig(Tx,DMA_IT_TC,ENABLE);
    if(Rx != NULL)
    DMA_ITConfig(Rx,DMA_IT_TC,ENABLE);

    if(Tx != NULL)
        DmaCH.UserCh |= (1 << tx);
    if(Rx != NULL)
        DmaCH.UserCh |= (1 << rx);

    if(Tx != NULL)
    DMA_Cmd(Rx, ENABLE);
    if(Rx != NULL)
    DMA_Cmd(Tx, ENABLE);

    rt_completion_wait(&(DmaCH.completion), RT_WAITING_FOREVER);
    DmaCH.IrqCh = 0;
    DmaCH.UserCh = 0;
}


int rt_hw_dma_init(void)
{
    rt_completion_init(&(DmaCH.completion));
    RT_LOGI(TAG,"DMA BUS Init");
    return 0;
}
INIT_PREV_EXPORT(rt_hw_dma_init);
