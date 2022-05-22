#include "flash.h"

//从指定地址开始写入多个数据（16位）
void FLASH_WriteHalfWordData(uint32_t startAddress, uint16_t *writeData, uint16_t countToWrite)
{
    uint32_t offsetAddress = startAddress - FLASH_BASE_ADDR; // 计算去掉0X08000000后的实际偏移地址
    uint32_t sectorPosition = offsetAddress / SECTOR_SIZE; // 计算扇区地址，对于STM32F103VET6为0~255
    uint32_t sectorStartAddress = sectorPosition * SECTOR_SIZE + FLASH_BASE_ADDR; // 对应扇区的首地址
    uint16_t dataIndex;

    if (startAddress < FLASH_BASE_ADDR || ((startAddress + countToWrite * 2) >= (FLASH_BASE_ADDR + SECTOR_SIZE * FLASH_SIZE)))
    {
        return; // 非法地址
    }
    FLASH_Unlock(); // 解锁写保护

    FLASH_ErasePage(sectorStartAddress); // 擦除这个扇区

    for (dataIndex = 0; dataIndex < countToWrite; dataIndex++)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, startAddress + dataIndex * 2, writeData[dataIndex]);
    }

    FLASH_Lock(); // 上锁写保护
}

//从指定地址开始写入多个数据（32位）
void FLASH_WriteWordData(uint32_t startAddress, uint32_t *writeData, uint16_t countToWrite)
{
    uint32_t offsetAddress = startAddress - FLASH_BASE_ADDR; // 计算去掉0X08000000后的实际偏移地址
    uint32_t sectorPosition = offsetAddress / SECTOR_SIZE; // 计算扇区地址，对于STM32F103VET6为0~255
    uint32_t sectorStartAddress = sectorPosition * SECTOR_SIZE + FLASH_BASE_ADDR; // 对应扇区的首地址
    uint16_t dataIndex;

    if (startAddress < FLASH_BASE_ADDR || ((startAddress + countToWrite * 4) >= (FLASH_BASE_ADDR + SECTOR_SIZE * FLASH_SIZE)))
    {
        return; // 非法地址
    }
    FLASH_Unlock(); // 解锁写保护

    FLASH_ErasePage(sectorStartAddress); // 擦除这个扇区

    for (dataIndex = 0; dataIndex < countToWrite; dataIndex++)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, startAddress + dataIndex * 4, writeData[dataIndex]);
    }

    FLASH_Lock(); // 上锁写保护
}

//从指定地址开始写入多个数据（64位）
void FLASH_WriteDoubleWordData(uint32_t startAddress, uint64_t *writeData, uint16_t countToWrite)
{
    uint32_t offsetAddress = startAddress - FLASH_BASE_ADDR; // 计算去掉0X08000000后的实际偏移地址
    uint32_t sectorPosition = offsetAddress / SECTOR_SIZE; // 计算扇区地址，对于STM32F103VET6为0~255
    uint32_t sectorStartAddress = sectorPosition * SECTOR_SIZE + FLASH_BASE_ADDR; // 对应扇区的首地址
    uint16_t dataIndex;

    if (startAddress < FLASH_BASE_ADDR || ((startAddress + countToWrite * 8) >= (FLASH_BASE_ADDR + SECTOR_SIZE * FLASH_SIZE)))
    {
        return; // 非法地址
    }
    FLASH_Unlock(); // 解锁写保护

    FLASH_ErasePage(sectorStartAddress); // 擦除这个扇区

    for (dataIndex = 0; dataIndex < countToWrite; dataIndex++)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, startAddress + dataIndex * 8, writeData[dataIndex]);
    }

    FLASH_Lock(); // 上锁写保护
}

// 读取指定地址的半字(16位数据)
uint16_t FLASH_ReadHalfWord(uint32_t address)
{
    return *(__IO uint16_t*)address;
}

// 读取指定地址的单字(32位数据)
uint16_t FLASH_ReadWord(uint32_t address)
{
    return *(__IO uint32_t*)address;
}

// 读取指定地址的双字(64位数据)
uint16_t FLASH_ReadDoubleWord(uint32_t address)
{
    return *(__IO uint64_t*)address;
}

// 从指定地址开始读取多个数据（16位数据）
void FLASH_ReadHalfWordData(uint32_t startAddress, uint16_t *readData, uint16_t countToRead)
{
    uint16_t dataIndex;
    for (dataIndex = 0; dataIndex < countToRead; dataIndex++)
    {
        readData[dataIndex] = FLASH_ReadHalfWord(startAddress + dataIndex * 2);
    }
}

// 从指定地址开始读取多个数据（32位数据）
void FLASH_ReadWordData(uint32_t startAddress, uint32_t *readData, uint16_t countToRead)
{
    uint16_t dataIndex;
    for (dataIndex = 0; dataIndex < countToRead; dataIndex++)
    {
        readData[dataIndex] = FLASH_ReadWord(startAddress + dataIndex * 4);
    }
}

// 从指定地址开始读取多个数据（64位数据）
void FLASH_ReadDoubleWordData(uint32_t startAddress, uint64_t *readData, uint16_t countToRead)
{
    uint16_t dataIndex;
    for (dataIndex = 0; dataIndex < countToRead; dataIndex++)
    {
        readData[dataIndex] = FLASH_ReadDoubleWord(startAddress + dataIndex * 8);
    }
}