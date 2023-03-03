#include "ff.h"
#include "25qxx.h"
#include "drv_log.h"

const static char *TAG = "Fatfs";

struct Fatfs_Dev{
    FATFS fs;          /* Pointer to the filesystem object to be registered (NULL:unmount)*/
};

static struct Fatfs_Dev Dev;

DIR dir;
FILINFO fno;
char PATCH[4] = {'0',':','/',0x00};
uint8_t scan_files(char * path)
{
    FRESULT res;
    res = f_opendir(&dir,path); //打开一个目录
    if (res == FR_OK)
    {
        while(1)
        {
            res = f_readdir(&dir, &fno);                   //读取目录下的一个文件
            if (res != FR_OK || fno.fname[0] == 0) break;  //错误了/到末尾了,退出
            if(fno.fname[0] != 0)
            {
                if(fno.fattrib == AM_DIR)
                    RT_LOGI(TAG,"%s/",fno.fname);
                else
                    RT_LOGI(TAG,"%s",fno.fname);
            }
            else if(fno.altname[0] != 0)
            {
                if(fno.fattrib == AM_DIR)
                    RT_LOGI(TAG,"%s/",fno.altname);
                else
                    RT_LOGI(TAG,"%s",fno.altname);
            }
        }
    }
    return res;
}

int FatfsApp_Init(void)
{
    RT_LOGI(TAG,"Fatfs UDisk Demo");
    RT_LOGI(TAG,"Mount:0:");
    f_mount(&Dev.fs, PATCH, 0);
    scan_files(PATCH);
    return 0;
}
INIT_ENV_EXPORT(FatfsApp_Init);
