#include "stm32f10x.h"                  // Device header
#include "Delay.h"


//这是软件I2C读取MPU6050

void SOFTWAER_I2C_READS_MPU6050_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_8,(BitAction)BitValue);
	Delay_us(10);
}

void SOFTWAER_I2C_READS_MPU6050_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_9,(BitAction)BitValue);
	Delay_us(10);
}

uint8_t SOFTWAER_I2C_READS_MPU6050_R_SDA(void)
{
	uint8_t BitValue;
	BitValue=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9);
	Delay_us(10);
	return BitValue;
}

void SOFTWAER_I2C_READS_MPU6050_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;//开漏输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_9);
}

void SOFTWAER_I2C_READS_MPU6050_Start(void)
{
	SOFTWAER_I2C_READS_MPU6050_W_SDA(1);
	SOFTWAER_I2C_READS_MPU6050_W_SCL(1);
	SOFTWAER_I2C_READS_MPU6050_W_SDA(0);
	SOFTWAER_I2C_READS_MPU6050_W_SCL(0);
}/*开始时，不过这个开始并不只是刚开始开始位，还有中间转换时的转换
开始时步骤:
SCL和SDA刚开始全是1(高电平)
开始时先释放SDA
再释放SCL组成开始开始位
{起始条件:SCL高电平期间,SDA从高电平切换到低电平}
中间转换时步骤:
先给SDA  1
再给SCL  1
是因为防止转换时被判断为停止位
{停止条件:SCL高电平期间,SDA从低电平切到高电平}
*/

void SOFTWAER_I2C_READS_MPU6050_Stop(void)
{
	SOFTWAER_I2C_READS_MPU6050_W_SDA(0);
	SOFTWAER_I2C_READS_MPU6050_W_SCL(1);
	SOFTWAER_I2C_READS_MPU6050_W_SDA(1);
}/*前一段结束
这时SCL会是低电平等待(读取段高电平时SDA不能反转)
但是SDA并不能确定是1|0
所以先将SDA回0
再进行结束位
{停止条件:SCL高电平期间,SDA从低电平切到高电平}
所以想被确定为标志位
先将SCL拉回高电平
再将SDA拉回高电平
完成结束位
*/

void SOFTWAER_I2C_READS_MPU6050_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i=0;i<8;i++)
	{
		SOFTWAER_I2C_READS_MPU6050_W_SDA(!!(Byte & (0x80 >> i)));
		//两个!可以对数据进行两次逻辑取反，作用是把非0值统一转换为1，即：!!(0) = 0，!!(非0) = 1
		//数电&知识点(此处在函数中转化为BitAction,具有非0即1的特点，所以最后结果即使是0x80，也会是1)
		//通过循环右移i位，实现数位运算
		SOFTWAER_I2C_READS_MPU6050_W_SCL(1);
		SOFTWAER_I2C_READS_MPU6050_W_SCL(0);
	}
}

uint8_t SOFTWAER_I2C_READS_MPU6050_ReceiveByte(void)
{
	uint8_t i;
	uint8_t Byte=0x00;
	SOFTWAER_I2C_READS_MPU6050_W_SDA(1);
	for (i=0;i<8;i++)
	{
		SOFTWAER_I2C_READS_MPU6050_W_SCL(1);
		if(SOFTWAER_I2C_READS_MPU6050_R_SDA()==1)
		{
			Byte |= (0x80>>i);
		}
		SOFTWAER_I2C_READS_MPU6050_W_SCL(0);
	}
	return Byte;
}
//以下为接收和发送应答位部分
void SOFTWAER_I2C_READS_MPU6050_SendACK(uint8_t ACKBit)
{

	SOFTWAER_I2C_READS_MPU6050_W_SDA(ACKBit);
	SOFTWAER_I2C_READS_MPU6050_W_SCL(1);
	SOFTWAER_I2C_READS_MPU6050_W_SCL(0);
}

uint8_t SOFTWAER_I2C_READS_MPU6050_ReceiveACK(void)
{
	uint8_t ACKBit;
	SOFTWAER_I2C_READS_MPU6050_W_SDA(1);
	SOFTWAER_I2C_READS_MPU6050_W_SCL(1);
	ACKBit=SOFTWAER_I2C_READS_MPU6050_R_SDA();
	SOFTWAER_I2C_READS_MPU6050_W_SCL(0);
	return ACKBit;
	//ACKBit读到1，表示从机没给应答
	//从机有义务将ACKBit置0，表示接收到
	//所以读0表示从机给了应答
}
