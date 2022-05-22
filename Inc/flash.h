#ifndef __FLASH_H__
#define __FLASH_H__

#include "main.h"

#define FLASH_BASE_ADDR		0x08000000UL		/* Flash����ַ */
#define SECTOR_SIZE			1024				/* ���С */
#define	FLASH_SIZE			(1*64*1024)			/* Flash ���� */
#define UserFlashAddress    ((uint32_t)(FLASH_BASE_ADDR | 0xFC00)) // �û���д��ʼ��ַ���ڲ�flash�����洢���ַ��0x0800FC00��ʼ��

uint16_t FLASH_ReadWord(uint32_t address);
void FLASH_WriteWordData(uint32_t startAddress, uint32_t *writeData, uint16_t countToWrite);

#endif /* __FLASH_H__ */