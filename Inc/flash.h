#ifndef __FLASH_H__
#define __FLASH_H__

#include "main.h"

#define FLASH_BASE_ADDR		0x08000000UL		/* Flash基地址 */
#define SECTOR_SIZE			1024				/* 块大小 */
#define	FLASH_SIZE			(1*64*1024)			/* Flash 容量 */
#define UserFlashAddress    ((uint32_t)(FLASH_BASE_ADDR | 0xFC00)) // 用户读写起始地址（内部flash的主存储块地址从0x0800FC00开始）

uint16_t FLASH_ReadWord(uint32_t address);
void FLASH_WriteWordData(uint32_t startAddress, uint32_t *writeData, uint16_t countToWrite);

#endif /* __FLASH_H__ */