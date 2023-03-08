#ifndef __FLASHALGO_H__
#define __FLASHALGO_H__

#include "stdint.h"

#define ARM_DRIFT_RAM               0X20000020
#define ARM_ADDR_RAM_SRAET          0X20000000
#define ARM_IRQ_HALT_LEN            8
typedef struct {
    uint32_t breakpoint;        // BKPT : start of blob + 1(sram起始地址+1)
    uint32_t static_base;       // RSB  : address to access global/static data(全局/静态变量存储地址)
    uint32_t stack_pointer;     // RSP  : stack pointer(堆栈指针)
}program_syscall_t;


typedef struct {
    uint32_t  init;             // 初始化函数地址
    uint32_t  uninit;           // 卸载初始化函数地址
    uint32_t  erase_chip;       // 全片擦除函数地址
    uint32_t  erase_sector;     // 按分区大小擦除函数地址
    uint32_t  program_page;     // 烧录函数地址
    program_syscall_t sys_call_s;
    uint32_t  program_buffer;   // 烧录算法缓存地址
    uint32_t  algo_start;       // 烧录算法起始地址
    uint32_t  algo_size;        // 烧录算法大小
    uint32_t *algo_blob;        // 烧录算法程序
    uint32_t  program_buffer_size;//烧录页大小
    uint32_t  algo_flags;       // 烧录算法标志位
}program_target_t;

typedef struct {
    const uint32_t start;
    const uint32_t size;
} sector_info_t;

typedef struct {
    uint32_t name[128];         //烧录算法名字(一般为芯片类型)
    uint32_t flash_start;       //芯片flash起始地址
    uint32_t flash_size;        //芯片flash大小
    uint32_t page_size;         //芯片编程页大小
    uint32_t sector_size;       //页大小
}chip_information;

struct FLM_DEV
{
	uint16_t magic;
	char name[128];
	uint16_t devType;
	uint32_t devAdr;
	uint32_t szDev;
	uint32_t szPage;
	uint32_t res;
	uint8_t  valEmpty;
	uint32_t toProg;
	uint32_t toErase;
	uint32_t szSector;
	uint32_t adrSector;
	uint32_t data11;
	uint32_t data12;
};
int FLM_Prase(const void *FName,program_target_t *target, chip_information *info);
#endif
