#include "ff.h"
#include "drv_log.h"
#include "flashalgo.h"
#include "swd_host.h"
#include "target_config.h"
#include "stdlib.h"
const static char *TAG = "Flashalgo";

FIL USERFile;
uint8_t bin_buff[2048];
uint8_t FLASH_SWD(char *File){
    uint8_t readflag = 0;
    uint8_t log = 10;
    uint32_t addr;
    uint16_t bytesread;
    FRESULT Res = f_open(&USERFile, File,FA_READ);
    if (Res == FR_OK)
    {
        readflag = 1;
      addr = 0;

        if (swd_init_debug())
        {
            if (target_flash_init(0x0000000) == ERROR_SUCCESS)
            {
                if (target_flash_erase_chip() == ERROR_SUCCESS)
                {
                    while(readflag){
                         f_read(&USERFile, bin_buff, 2048, (void *)&bytesread);
                         if(bytesread < 2048){
                             readflag = 0;
                         }
                            if (target_flash_program_page(0x0000000 + addr, &bin_buff[0], 2048) == ERROR_SUCCESS)
                        {
                             uint32_t progess = (((double)addr/f_size(&USERFile))*100);
                             if(progess >= log){
                                 RT_LOGI(TAG,"%d%%",log);
                                 log += 10;
                             }
                        }else return 0;
                        addr += 2048;
                    }
                    if (swd_init_debug())
                    {
                         swd_set_target_reset(0);
                         rt_thread_mdelay(100);
                         return 1;
                    }
                    else return 0;
                }else return 0;
            }
            target_flash_uninit();
        }else return 0;
    }else return 0;
    return 0;
}



//static struct SWD_Function swd = {
//    .reset = 0,
//    .
//};

static char FILENAME[] = "0:/STM32F10x_512.FLM";
int flashalgo_test(void)
{
    program_target_t target;
    chip_information info;
    FLM_Prase(FILENAME, &target, &info);
    rt_kprintf("name:%s \r\n",info.name);
    rt_kprintf("flash_start:%#x \r\n",info.flash_start);
    rt_kprintf("flash_size:%#x \r\n",info.flash_size);
    rt_kprintf("page_size:%#x \r\n",info.page_size);
    rt_kprintf("sector_size:%#x \r\n",info.sector_size);
    rt_kprintf("algo_size:%d \r\n",target.algo_size);
    for(int i = 0;i<target.algo_size/sizeof(uint32_t);i++)
    {
        if(i%8 == 0)
            rt_kprintf("\r\n");
        rt_kprintf("0X%08X ",target.algo_blob[i]);
    }

    rt_kprintf("\r\n");
    rt_kprintf("init:0X%08X\r\n",target.init);
    rt_kprintf("uninit:0X%08X\r\n",target.uninit);
    rt_kprintf("erase_chip:0X%08X\r\n",target.erase_chip);
    rt_kprintf("erase_sector:0X%08X\r\n",target.erase_sector);
    rt_kprintf("program_page:0X%08X\r\n",target.program_page);
    rt_kprintf("program_buffer_size:0X%08X\r\n",target.program_buffer_size);
    free(target.algo_blob);
}

int FlashalgoApp_Init(void)
{
    RT_LOGI(TAG,"Flashalgo Init");
//    RT_LOGI();
    flashalgo_test();
    return 0;
}
INIT_APP_EXPORT(FlashalgoApp_Init);
