/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-15     SummerGift   first version
 */

/*
 * NOTE: DO NOT include this file on the header file.
 */
#ifndef __DRV_LOG_H
#define __DRV_LOG_H

#include "rtthread.h"
#include "ch32v20x.h"

#ifndef LOG_TAG
#define DBG_TAG               "drv"
#else
#define DBG_TAG               LOG_TAG
#endif /* LOG_TAG */

#ifdef DRV_DEBUG
#define DBG_LVL               DBG_LOG
#else
#define DBG_LVL               DBG_INFO
#endif /* DRV_DEBUG */

#define CHIP_ID_LEN             12

#define RT_LOGW(TAG,fmt, args...)     rt_kprintf("\033[1;33m(%05d) %s: "fmt"\e[0m\n",rt_tick_get(),TAG,##args)
#define RT_LOGI(TAG,fmt, args...)     rt_kprintf("\033[1;32m(%05d) %s: "fmt"\e[0m\n",rt_tick_get(),TAG,##args)
#define RT_LOGE(TAG,fmt, args...)     rt_kprintf("\033[1;31m(%05d) %s: "fmt"\e[0m\n",rt_tick_get(),TAG,##args)
#define RT_LOGIHEX(TAG,DATA,LEN)      rt_kprintf("\033[1;32m(%05d) %s: ",rt_tick_get(),TAG);\
                                         for(int i = 0;i<LEN;i++){   \
                                           rt_kprintf("%#02x ",DATA[i]);}rt_kprintf("\e[0m\n")

#include <rtdbg.h>
#endif

