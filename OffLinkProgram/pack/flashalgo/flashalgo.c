#include <stdio.h>
#include "FLM.h"
#include "flashalgo.h"
#include <ff.h>
#include <unistd.h>
#include "string.h"
#include "stdlib.h"

#define FILENAME "./FlashHC32L19X_256K.FLM"
static FIL fp;
const char *StrFunNameTable[] = 
{
    "Init", 
    "UnInit", 
    "EraseChip", 
    "EraseSector", 
    "ProgramPage",
    NULL
};

const uint32_t irq_halt[8] = {
   	0xE00ABE00,
	0x062D780D,
	0x24084068,
	0xD3000040,
	0x1E644058,
	0x1C49D1FA,
	0x2A001E52,
	0x4770D1F2,
};

int ReadDataFromFile(const char *FName, uint32_t offset, void *buf, uint32_t size)
{
	int ret = 0;
	int bt = 0;

	if (f_open(&fp,FName, FA_READ) != FR_OK)
	{
		ret = -1;
		goto __exit;
	}

	if (f_lseek(&fp, offset) != FR_OK)
	{
		ret = -2;
		goto __exit;
	}

	if (f_read(&fp, buf, size, &bt) != FR_OK)
	{
		ret = -3;
		goto __exit;
	}

__exit:
	f_close(&fp);
	return ret;
}

// 读取程序头信息（Program Header）
int FLM_Program_Header(const void *FName,Elf32_Ehdr *ehdr,program_target_t *target,chip_information *info)
{
    Elf32_Phdr Phdr;
    struct FLM_DEV FLM_DEV;
    for(int i = 0; i< ehdr->e_phnum;i++)
    {
        ReadDataFromFile(FName, ehdr->e_phoff + (i * sizeof(Elf32_Phdr)), &Phdr, sizeof(Elf32_Phdr));
        if (Phdr.p_type == PT_LOAD && (Phdr.p_flags & (PF_X | PF_W | PF_R)) == (PF_X | PF_W | PF_R))
		{
            target->algo_blob = malloc(Phdr.p_filesz + sizeof(32) * ARM_IRQ_HALT_LEN);
            memcpy(target->algo_blob,irq_halt,sizeof(irq_halt));
			ReadDataFromFile(FName, Phdr.p_offset, &target->algo_blob[ARM_IRQ_HALT_LEN], Phdr.p_filesz); // 提取需要下载到RAM的程序代码
			target->algo_size = Phdr.p_filesz + sizeof(32) * ARM_IRQ_HALT_LEN;
		}
 		else if (Phdr.p_type == PT_LOAD && (Phdr.p_flags & (PF_R)) == (PF_R))
		{
			ReadDataFromFile(FName, Phdr.p_offset, &FLM_DEV, sizeof(FLM_DEV));
            memcpy(info->name , FLM_DEV.name, sizeof(FLM_DEV.name));
            info->flash_start = FLM_DEV.devAdr;
            info->flash_size = FLM_DEV.szDev;
            info->page_size = FLM_DEV.szPage;
            info->sector_size = FLM_DEV.szSector;
		}   
    }

    return 0;
}


int FLM_Sections_Value(const void *FName,Elf32_Shdr *Shdr,program_target_t *target, int num, int len)
{
    Elf32_Sym Sym;
    for(int i = 0;i< Shdr->sh_size / sizeof(Elf32_Sym);i++)
    {
        ReadDataFromFile(FName, Shdr->sh_offset + (i * sizeof(Elf32_Sym)), &Sym, sizeof(Elf32_Sym));
        if(len == Sym.st_name)
        {
           switch(num)
           {
                case 0:
                    target->init = Sym.st_value + ARM_DRIFT_RAM;
                    break;
                case 1:
                    target->uninit = Sym.st_value + ARM_DRIFT_RAM;
                    break;
                case 2:
                    target->erase_chip = Sym.st_value + ARM_DRIFT_RAM;
                    break;
                case 3:
                    target->erase_sector = Sym.st_value + ARM_DRIFT_RAM;
                    break;
                case 4:
                    target->program_page = Sym.st_value + ARM_DRIFT_RAM;
                    break;
           }
        }
    }

    return 0;
}
// 读取节区头部（Sections Header）
int FLM_Sections_Header(const void *FName,Elf32_Ehdr *ehdr,program_target_t *target)
{
    Elf32_Shdr Shdr;
    Elf32_Shdr ShdrSym;
    
    for (int i = 0; i < ehdr->e_shnum; i++)
	{
        ReadDataFromFile(FName, ehdr->e_shoff + (i * sizeof(Elf32_Shdr)), &Shdr, sizeof(Elf32_Shdr));
		if (Shdr.sh_type == SHT_SYMTAB)
		{
			memcpy(&ShdrSym, &Shdr, sizeof(Elf32_Shdr));
            ReadDataFromFile(FName, ehdr->e_shoff + (ShdrSym.sh_link * sizeof(Elf32_Shdr)), &Shdr, sizeof(Elf32_Shdr));
			// 查找字符串表头并拷贝出来备用
			if (Shdr.sh_type == SHT_STRTAB)
			{
				break;
			}
		}
	}
    uint8_t *ShdrStr = malloc(Shdr.sh_size);
    uint8_t *Str = ShdrStr;
    ReadDataFromFile(FName, Shdr.sh_offset, ShdrStr, Shdr.sh_size);
	for(int i = 0;i < Shdr.sh_size;i++)
    {
        if(ShdrStr[i] == 0x00)
        {
            int j = 0;
            while(StrFunNameTable[j] != NULL)
            {
                if((strlen(StrFunNameTable[j]) == strlen(Str)) && ((strncmp(StrFunNameTable[j],Str,sizeof(StrFunNameTable[j]))) == 0))
                {
                    FLM_Sections_Value(FName, &ShdrSym, target,j,i - strlen(Str));
                    break;
                }
                j++;
            }
            j = strlen(Str);
            Str += j;
            Str++;
        }
    }
    free(ShdrStr);
    return 0;
}

int FLM_Prase(const void *FName,program_target_t *target, chip_information *info)
{
    Elf32_Ehdr ehdr;	// ELF文件信息头

    ReadDataFromFile(FName, 0, &ehdr, sizeof(Elf32_Ehdr));

	// 不是ELF格式文件
	if (strstr((const char *)ehdr.e_ident, "ELF") == NULL)
	{
		return -1;
	}
    FLM_Program_Header(FName, &ehdr, target, info);
    FLM_Sections_Header(FName, &ehdr, target);


    target->sys_call_s.breakpoint = ARM_ADDR_RAM_SRAET + 1;
    target->sys_call_s.static_base = ARM_ADDR_RAM_SRAET + 0xC00;
    target->sys_call_s.stack_pointer = ARM_ADDR_RAM_SRAET + 0x1000;
    target->program_buffer = ARM_ADDR_RAM_SRAET + 400;
    target->algo_start = ARM_ADDR_RAM_SRAET;
    target->program_buffer_size = info->page_size;
    return 0;
}


//
//int main(int argc, char const *argv[])
//{
//    program_target_t target;
//    chip_information info;
//    FLM_Prase(FILENAME, &target, &info);
//    printf("name:%s \r\n",info.name);
//    printf("flash_start:%#x \r\n",info.flash_start);
//    printf("flash_size:%#x \r\n",info.flash_size);
//    printf("sector_size:%#x \r\n",info.page_size);
//    printf("algo_size:%d \r\n",target.algo_size);
//    for(int i = 0;i<target.algo_size/sizeof(uint32_t);i++)
//    {
//        if(i%8 == 0)
//            printf("\r\n");
//        printf("0X%08X ",target.algo_blob[i]);
//    }
//
//    printf("\r\n");
//    printf("init:0X%08X\r\n",target.init);
//    printf("uninit:0X%08X\r\n",target.uninit);
//    printf("erase_chip:0X%08X\r\n",target.erase_chip);
//    printf("erase_sector:0X%08X\r\n",target.erase_sector);
//    printf("program_page:0X%08X\r\n",target.program_page);
//    printf("program_buffer_size:0X%08X\r\n",target.program_buffer_size);
//    free(target.algo_blob);
//}
