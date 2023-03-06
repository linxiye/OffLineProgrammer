#include "drivers/pin.h"
#include "rtthread.h"
#include "drv_log.h"
#include "rthw.h"

const static char *TAG = "SWD";

#define SWCLK       54
#define SWDIO       53



void Swd_reset(void)
{
    for(int i = 0;i< 51;i++)
    {
        rt_pin_write(SWCLK,1);
        rt_hw_us_delay(1);
        rt_pin_write(SWCLK,0);
        rt_hw_us_delay(1);
    }
}


int SWDApp_Init(void)
{
    rt_pin_mode(SWCLK, PIN_MODE_OUTPUT);
    rt_pin_mode(SWDIO, PIN_MODE_OUTPUT);
    rt_pin_write(SWCLK,1);
    rt_pin_write(SWDIO,1);


    RT_LOGI(TAG,"SWDApp Init");
    return 0;
}
//INIT_APP_EXPORT(SWDApp_Init);
