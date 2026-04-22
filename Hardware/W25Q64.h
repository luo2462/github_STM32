#ifndef __W25Q64_H
#define __W25Q64_H

typedef struct{
uint8_t MID;//厂商ID
uint32_t DID;//设备ID
}ID;


void W25Q64_Init(void);
void W25Q64_ReadID(ID *ID);
void W25Q64_WriteEnable(void);//写使能
void W25Q64_WaitBusy(void);//等待Busy位
void W25Q62_PageProgram(uint32_t Address,uint8_t *DataArray,uint16_t count);//页编程
void W25Q64_ChipErase(void);//W25Q64 全片擦除函数
void W25Q64_BlockErase64K(uint32_t Address);//W25Q64 64KB块擦除函数
void W25Q64_BlockErase32K(uint32_t Address);//W25Q64 32KB块擦除函数
void W25Q64_SectorErase(uint32_t Address);//W25Q64 页擦除(4KB 扇区擦除)
void W25Q64_EraseSuspend(void);//W25Q64 擦除暂停函数
void W25Q64_EraseResume(void);//W25Q64 擦除恢复函数
void W25Q64_ReadData(uint32_t Address,uint8_t *DataArray,uint32_t count);//读取数据

#endif
