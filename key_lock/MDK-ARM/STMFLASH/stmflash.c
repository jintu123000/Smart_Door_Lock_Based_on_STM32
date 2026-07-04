#include "stmflash.h"

void passwd_flash_erase(void)
{
	uint32_t page_error;
	HAL_FLASH_Unlock();
	FLASH_EraseInitTypeDef EraseInitStruct;
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = FLASH_USER_START_ADDR;
    EraseInitStruct.NbPages = (FLASH_USER_END_ADDR - FLASH_USER_START_ADDR) / FLASH_PAGE_SIZE;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &page_error) != HAL_OK)
    {
        while (1)
        {
			printf("Flash erase error!\n");
			HAL_Delay(1000);
        }
    }
	HAL_FLASH_Lock();
}

void passwd_flash_write(u32 data_write)
{
	HAL_FLASH_Unlock();
	uint32_t Address = FLASH_USER_START_ADDR;
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, data_write);
	HAL_FLASH_Lock();
}

u32 passwd_flash_read(void)
{
	HAL_FLASH_Unlock();
	uint32_t Address = FLASH_USER_START_ADDR;
    HAL_FLASH_Lock();
	return *(__IO uint32_t *)Address;
}

