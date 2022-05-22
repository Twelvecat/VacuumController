#include "flash.h"

//��ָ����ַ��ʼд�������ݣ�16λ��
void FLASH_WriteHalfWordData(uint32_t startAddress, uint16_t *writeData, uint16_t countToWrite)
{
    uint32_t offsetAddress = startAddress - FLASH_BASE_ADDR; // ����ȥ��0X08000000���ʵ��ƫ�Ƶ�ַ
    uint32_t sectorPosition = offsetAddress / SECTOR_SIZE; // ����������ַ������STM32F103VET6Ϊ0~255
    uint32_t sectorStartAddress = sectorPosition * SECTOR_SIZE + FLASH_BASE_ADDR; // ��Ӧ�������׵�ַ
    uint16_t dataIndex;

    if (startAddress < FLASH_BASE_ADDR || ((startAddress + countToWrite * 2) >= (FLASH_BASE_ADDR + SECTOR_SIZE * FLASH_SIZE)))
    {
        return; // �Ƿ���ַ
    }
    FLASH_Unlock(); // ����д����

    FLASH_ErasePage(sectorStartAddress); // �����������

    for (dataIndex = 0; dataIndex < countToWrite; dataIndex++)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, startAddress + dataIndex * 2, writeData[dataIndex]);
    }

    FLASH_Lock(); // ����д����
}

//��ָ����ַ��ʼд�������ݣ�32λ��
void FLASH_WriteWordData(uint32_t startAddress, uint32_t *writeData, uint16_t countToWrite)
{
    uint32_t offsetAddress = startAddress - FLASH_BASE_ADDR; // ����ȥ��0X08000000���ʵ��ƫ�Ƶ�ַ
    uint32_t sectorPosition = offsetAddress / SECTOR_SIZE; // ����������ַ������STM32F103VET6Ϊ0~255
    uint32_t sectorStartAddress = sectorPosition * SECTOR_SIZE + FLASH_BASE_ADDR; // ��Ӧ�������׵�ַ
    uint16_t dataIndex;

    if (startAddress < FLASH_BASE_ADDR || ((startAddress + countToWrite * 4) >= (FLASH_BASE_ADDR + SECTOR_SIZE * FLASH_SIZE)))
    {
        return; // �Ƿ���ַ
    }
    FLASH_Unlock(); // ����д����

    FLASH_ErasePage(sectorStartAddress); // �����������

    for (dataIndex = 0; dataIndex < countToWrite; dataIndex++)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, startAddress + dataIndex * 4, writeData[dataIndex]);
    }

    FLASH_Lock(); // ����д����
}

//��ָ����ַ��ʼд�������ݣ�64λ��
void FLASH_WriteDoubleWordData(uint32_t startAddress, uint64_t *writeData, uint16_t countToWrite)
{
    uint32_t offsetAddress = startAddress - FLASH_BASE_ADDR; // ����ȥ��0X08000000���ʵ��ƫ�Ƶ�ַ
    uint32_t sectorPosition = offsetAddress / SECTOR_SIZE; // ����������ַ������STM32F103VET6Ϊ0~255
    uint32_t sectorStartAddress = sectorPosition * SECTOR_SIZE + FLASH_BASE_ADDR; // ��Ӧ�������׵�ַ
    uint16_t dataIndex;

    if (startAddress < FLASH_BASE_ADDR || ((startAddress + countToWrite * 8) >= (FLASH_BASE_ADDR + SECTOR_SIZE * FLASH_SIZE)))
    {
        return; // �Ƿ���ַ
    }
    FLASH_Unlock(); // ����д����

    FLASH_ErasePage(sectorStartAddress); // �����������

    for (dataIndex = 0; dataIndex < countToWrite; dataIndex++)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, startAddress + dataIndex * 8, writeData[dataIndex]);
    }

    FLASH_Lock(); // ����д����
}

// ��ȡָ����ַ�İ���(16λ����)
uint16_t FLASH_ReadHalfWord(uint32_t address)
{
    return *(__IO uint16_t*)address;
}

// ��ȡָ����ַ�ĵ���(32λ����)
uint16_t FLASH_ReadWord(uint32_t address)
{
    return *(__IO uint32_t*)address;
}

// ��ȡָ����ַ��˫��(64λ����)
uint16_t FLASH_ReadDoubleWord(uint32_t address)
{
    return *(__IO uint64_t*)address;
}

// ��ָ����ַ��ʼ��ȡ������ݣ�16λ���ݣ�
void FLASH_ReadHalfWordData(uint32_t startAddress, uint16_t *readData, uint16_t countToRead)
{
    uint16_t dataIndex;
    for (dataIndex = 0; dataIndex < countToRead; dataIndex++)
    {
        readData[dataIndex] = FLASH_ReadHalfWord(startAddress + dataIndex * 2);
    }
}

// ��ָ����ַ��ʼ��ȡ������ݣ�32λ���ݣ�
void FLASH_ReadWordData(uint32_t startAddress, uint32_t *readData, uint16_t countToRead)
{
    uint16_t dataIndex;
    for (dataIndex = 0; dataIndex < countToRead; dataIndex++)
    {
        readData[dataIndex] = FLASH_ReadWord(startAddress + dataIndex * 4);
    }
}

// ��ָ����ַ��ʼ��ȡ������ݣ�64λ���ݣ�
void FLASH_ReadDoubleWordData(uint32_t startAddress, uint64_t *readData, uint16_t countToRead)
{
    uint16_t dataIndex;
    for (dataIndex = 0; dataIndex < countToRead; dataIndex++)
    {
        readData[dataIndex] = FLASH_ReadDoubleWord(startAddress + dataIndex * 8);
    }
}