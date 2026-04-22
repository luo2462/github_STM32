#include "stm32f10x.h"                  // Device header
#include "W25Q64.h"
#include "W25Q64_ins.h"
#include "SoftWare_SPI.h"


void W25Q64_Init(void)
{
	SoftWare_SPI_Init();
}

void W25Q64_ReadID(ID *ID)
{
	SoftWare_SPI_START();
	SoftWare_SPI_mode_0_SwapByte(W25Q64_JEDEC_ID);

	//厂商ID
	ID->MID=SoftWare_SPI_mode_0_SwapByte(W25Q64_DUMMY_BYTE);//0xFF没有意义。纯置换对方数据
	//设备ID
	ID->DID=SoftWare_SPI_mode_0_SwapByte(W25Q64_DUMMY_BYTE);//获取高八位
	ID->DID<<=8;//左移8位使高位数据去往高位
	ID->DID |= SoftWare_SPI_mode_0_SwapByte(W25Q64_DUMMY_BYTE);//低八位(本来全0,用|=有1为1)
	SoftWare_SPI_STOP();
}

void W25Q64_WriteEnable(void)
{
	SoftWare_SPI_START();
	SoftWare_SPI_mode_0_SwapByte(W25Q64_WRITE_ENABLE);
	SoftWare_SPI_STOP();
}//写使能

void W25Q64_WaitBusy(void)
{
	uint32_t TimeOut=100000;
	SoftWare_SPI_START();
	SoftWare_SPI_mode_0_SwapByte(W25Q64_READ_STATUS_REGISTER_1);
	while((SoftWare_SPI_mode_0_SwapByte(W25Q64_DUMMY_BYTE) & 0x01)==0x01)
	{
		TimeOut--;
		if(TimeOut==0)
		{
			break;
		}
	}
	SoftWare_SPI_STOP();
}//等待Busy位

void W25Q62_PageProgram(uint32_t Address,uint8_t *DataArray,uint16_t count)
{
	W25Q64_WriteEnable();
	uint16_t i;
	SoftWare_SPI_START();
	SoftWare_SPI_mode_0_SwapByte(W25Q64_PAGE_PROGRAM);
	//24位地址，每次只能发送8位
	SoftWare_SPI_mode_0_SwapByte(Address>>16);//高位字节
	SoftWare_SPI_mode_0_SwapByte(Address>>8);//中间字节
	SoftWare_SPI_mode_0_SwapByte(Address);//低位字节
	for(i=0;i<count;i++)
	{
		SoftWare_SPI_mode_0_SwapByte(DataArray[i]);
	}
	SoftWare_SPI_STOP();
	W25Q64_WaitBusy();
}//页编程

/**
 * @brief  W25Q64 全片擦除函数
 * @param  无
 * @retval 无
 */
void W25Q64_ChipErase(void)
{
	W25Q64_WriteEnable();
    SoftWare_SPI_START();
    // 发送全片擦除指令码 0xC7（或0x60，二选一，W25Q64兼容两种）
    SoftWare_SPI_mode_0_SwapByte(W25Q64_CHIP_ERASE);
    SoftWare_SPI_STOP();
	W25Q64_WaitBusy();
}

/**
 * @brief  W25Q64 64KB块擦除函数
 * @param  Address: 要擦除的64KB块的起始地址（必须64KB对齐，即0x10000的整数倍）
 * @retval 无
 */
void W25Q64_BlockErase64K(uint32_t Address)
{
	W25Q64_WriteEnable();
    SoftWare_SPI_START();
    // 发送64KB块擦除指令码 0xD8
    SoftWare_SPI_mode_0_SwapByte(W25Q64_BLOCK_ERASE_64KB);
    // 发送24位地址（高位到低位）
    SoftWare_SPI_mode_0_SwapByte(Address>>16);
    SoftWare_SPI_mode_0_SwapByte(Address>>8);
    SoftWare_SPI_mode_0_SwapByte(Address);
    SoftWare_SPI_STOP();
	W25Q64_WaitBusy();
}

/**
 * @brief  W25Q64 32KB块擦除函数
 * @param  Address: 要擦除的32KB块的起始地址（必须32KB对齐，即0x8000的整数倍）
 * @retval 无
 */
void W25Q64_BlockErase32K(uint32_t Address)
{
	W25Q64_WriteEnable();
    SoftWare_SPI_START();
    // 发送32KB块擦除指令码 0x52
    SoftWare_SPI_mode_0_SwapByte(W25Q64_BLOCK_ERASE_32KB);
    // 发送24位地址（高位到低位）
    SoftWare_SPI_mode_0_SwapByte(Address>>16);
    SoftWare_SPI_mode_0_SwapByte(Address>>8);
    SoftWare_SPI_mode_0_SwapByte(Address);
    SoftWare_SPI_STOP();
	W25Q64_WaitBusy();
}

/**
 * @brief  W25Q64 页擦除(4KB 扇区擦除)
 * @param  Address: 要擦除的4KB块的起始地址
 * @retval 无
 */
void W25Q64_SectorErase(uint32_t Address)
{
	W25Q64_WriteEnable();
	SoftWare_SPI_START();
	SoftWare_SPI_mode_0_SwapByte(W25Q64_SECTOR_ERASE_4KB);
	SoftWare_SPI_mode_0_SwapByte(Address>>16);
	SoftWare_SPI_mode_0_SwapByte(Address>>8);
	SoftWare_SPI_mode_0_SwapByte(Address);
	SoftWare_SPI_STOP();
	W25Q64_WaitBusy();
}

/**
 * @brief  W25Q64 擦除暂停函数
 * @param  无
 * @retval 无
 */
void W25Q64_EraseSuspend(void)
{
	W25Q64_WriteEnable();
    SoftWare_SPI_START();
    // 发送擦除暂停指令码 0x75
    SoftWare_SPI_mode_0_SwapByte(W25Q64_ERASE_SUSPEND);
    SoftWare_SPI_STOP();
	W25Q64_WaitBusy();
}

/**
 * @brief  W25Q64 擦除恢复函数
 * @param  无
 * @retval 无
 */
void W25Q64_EraseResume(void)
{
	W25Q64_WriteEnable();
    SoftWare_SPI_START();
    // 发送擦除恢复指令码 0x7A
    SoftWare_SPI_mode_0_SwapByte(W25Q64_ERASE_RESUME);
    SoftWare_SPI_STOP();
	W25Q64_WaitBusy();
}

void W25Q64_ReadData(uint32_t Address,uint8_t *DataArray,uint32_t count)
{
	uint32_t i;
	SoftWare_SPI_START();
	SoftWare_SPI_mode_0_SwapByte(W25Q64_READ_DATA);
	//24位地址，每次只能发送8位
	SoftWare_SPI_mode_0_SwapByte(Address>>16);//高位字节
	SoftWare_SPI_mode_0_SwapByte(Address>>8);//中间字节
	SoftWare_SPI_mode_0_SwapByte(Address);//低位字节
	for(i=0;i<count;i++)
	{
		DataArray[i]=SoftWare_SPI_mode_0_SwapByte(W25Q64_DUMMY_BYTE);
	}
	SoftWare_SPI_STOP();
}//读取数据
