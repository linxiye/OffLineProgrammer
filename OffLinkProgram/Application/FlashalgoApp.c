#include "ff.h"
#include "drv_log.h"
#include "flashalgo.h"
#include "stdlib.h"
#include "drivers/pin.h"
#include "rthw.h"
#include "target_config.h"
#include "swd_host.h"
const static char *TAG = "Flashalgo";


char FILENAME[] = "0:/STM32F10x_512.FLM";
char BINNAME[] = "0:/LED.bin";
program_target_t flash_algo;
int flashalgo_test(void)
{

    chip_information info;
    FLM_Prase(FILENAME, &flash_algo, &info);
//    rt_kprintf("name:%s \r\n",info.name);
//    rt_kprintf("flash_start:%#x \r\n",info.flash_start);
//    rt_kprintf("flash_size:%#x \r\n",info.flash_size);
//    rt_kprintf("page_size:%#x \r\n",info.page_size);
//    rt_kprintf("sector_size:%#x \r\n",info.sector_size);
//    rt_kprintf("algo_size:%d \r\n",flash_algo.algo_size);
//    for(int i = 0;i<flash_algo.algo_size/sizeof(uint32_t);i++)
//    {
//        if(i%8 == 0)
//            rt_kprintf("\r\n");
//        rt_kprintf("0X%08X ",flash_algo.algo_blob[i]);
//    }
//
//    rt_kprintf("\r\n");
//    rt_kprintf("init:0X%08X\r\n",flash_algo.init);
//    rt_kprintf("uninit:0X%08X\r\n",flash_algo.uninit);
//    rt_kprintf("erase_chip:0X%08X\r\n",flash_algo.erase_chip);
//    rt_kprintf("erase_sector:0X%08X\r\n",flash_algo.erase_sector);
//    rt_kprintf("program_page:0X%08X\r\n",flash_algo.program_page);
//    rt_kprintf("program_buffer_size:0X%08X\r\n",flash_algo.program_buffer_size);
//    free(flash_algo.algo_blob);
}
#define DLSize      1024
FIL USERFile;

uint8_t FLASH_SWD(char *File){
    uint8_t readflag = 0;
    uint32_t addr;
    int bytesread;
    uint8_t *bin_buff = rt_malloc(DLSize);
    FRESULT Res = f_open(&USERFile, File,FA_READ);
    if (Res == FR_OK)
    {
        readflag = 1;
      addr = 0;

        if (swd_init_debug())
        {
            if (target_flash_init(0x8000000) == ERROR_SUCCESS)
            {

                if (target_flash_erase_chip() == ERROR_SUCCESS)
                {
                    RT_LOGI(TAG,"target_flash_erase_chip");
                    while(readflag){
                         f_read(&USERFile, bin_buff, DLSize, &bytesread);
                         if(bytesread<DLSize){
                             readflag = 0;
                         }
                            if (target_flash_program_page(0x8000000 + addr, &bin_buff[0], DLSize) == ERROR_SUCCESS)
                        {
                             uint32_t progess = (((double)addr/f_size(&USERFile))*100);
                             RT_LOGI(TAG,"%d%%",progess);
                        }else return 0;
                        addr += DLSize;
                    }
                    if (swd_init_debug())
                    {
                         swd_set_target_reset(0);//
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

int FlashalgoApp_Init(void)
{
    RT_LOGI(TAG,"Flashalgo Init");
    flashalgo_test();
    FLASH_SWD(BINNAME);
    return 0;
}
INIT_APP_EXPORT(FlashalgoApp_Init);
