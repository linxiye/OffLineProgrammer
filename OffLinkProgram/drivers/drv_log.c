#include "drv_log.h"

const static char *TAG = "LOG";

#define FLASH_SIZE  *((uint16_t*)0x1FFFF7E0)
#define Chip_Addr    (0x1FFFF7E8)

void Ch32_Chid_Id_Read(uint8_t *Id,uint8_t count)
{
    uint32_t addr = Chip_Addr;
    while(count--)
    {
        *(Id++) = *((uint8_t *)addr++);
    }
}


int Ch32_Log_Init(void)
{
    uint8_t ChipId[CHIP_ID_LEN];
    RCC_ClocksTypeDef RCC_ClocksStatus={0};
    RCC_GetClocksFreq(&RCC_ClocksStatus);
    RT_LOGI(TAG,"MCU: CH32V208WBU6");
    RT_LOGI(TAG,"Flash Size:%dK Sram Size:%dK",FLASH_SIZE,64);
    RT_LOGI(TAG,"SYSCLK_Frequency:%d", RCC_ClocksStatus.SYSCLK_Frequency);
    RT_LOGI(TAG,"HCLK_Frequency:%d", RCC_ClocksStatus.HCLK_Frequency);
    RT_LOGI(TAG,"PCLK1_Frequency:%d", RCC_ClocksStatus.PCLK1_Frequency);
    RT_LOGI(TAG,"PCLK2_Frequency:%d", RCC_ClocksStatus.PCLK2_Frequency);
    Ch32_Chid_Id_Read(&ChipId[0],CHIP_ID_LEN);
    RT_LOGI(TAG,"Chip Id:%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",ChipId[0],ChipId[1],ChipId[2],
            ChipId[3],ChipId[4],ChipId[5],ChipId[6],ChipId[7],ChipId[8],ChipId[9],ChipId[10],ChipId[11]);

    return RT_EOK;
}
INIT_PREV_EXPORT(Ch32_Log_Init);
