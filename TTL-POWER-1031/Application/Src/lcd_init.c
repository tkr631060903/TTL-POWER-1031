#include "lcd_init.h"
#include "Application.h"
#include "UART_Debug.h"
#include <string.h>
#include "Application_LCD.h"

extern SPI_HandleTypeDef hspi2;
static uint8_t LCD_Buff[LCD_BUFF_SIZE];
static uint16_t LCD_Buff_Point = 0;
uint8_t isData = 0;  // 0:命令  1:数据
uint8_t USE_HORIZONTAL = 0;

void delay(uint16_t time)
{
	uint16_t i = 0;
    while (time--)
    {
        i = 9060;  //自己定义
        while (i--);
    }
}
/******************************************************************************
	  函数说明：LCD串行数据写入函数
	  入口数据：dat  要写入的串行数据
	  返回值：  无
******************************************************************************/
void LCD_Writ_Bus(uint8_t dat)
{
	extern DMA_HandleTypeDef hdma_spi2_tx;
	if (isData)
    {
        if(LCD_Buff_Point == LCD_BUFF_SIZE - 1)
        {
			HAL_SPI_Transmit_DMA(&hspi2, LCD_Buff, LCD_BUFF_SIZE);
            while (HAL_DMA_GetState(&hdma_spi2_tx) != HAL_DMA_STATE_READY);
			memset(LCD_Buff, 0, LCD_BUFF_SIZE);
            LCD_Buff_Point = 0;
        }else{
            LCD_Buff[LCD_Buff_Point] = dat;
            LCD_Buff_Point++;
        }
    }else{
        if (LCD_Buff_Point != 0)
        {
			HAL_SPI_Transmit_DMA(&hspi2, LCD_Buff, LCD_Buff_Point);
            while (HAL_DMA_GetState(&hdma_spi2_tx) != HAL_DMA_STATE_READY);
			memset(LCD_Buff, 0, LCD_Buff_Point);
            LCD_Buff_Point = 0;
        }
		HAL_SPI_Transmit(&hspi2, &dat, 1, 10);//发送
		while (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY);//检查接收标志位
	}

	// while (HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_RESET);
	// HAL_SPI_Transmit(&hspi2, &dat, 1, 10);
	// HAL_SPI_Transmit_DMA(&hspi2, &dat, 1);
}

/******************************************************************************
	  函数说明：LCD写入数据
	  入口数据：dat 写入的数据
	  返回值：  无
******************************************************************************/
void LCD_WR_DATA8(uint8_t dat)
{
	LCD_Writ_Bus(dat);
}


void LCD_WR_DATA8_NO_DMA(uint8_t dat)
{
	while (HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_RESET);//检查接收标志位
	HAL_SPI_Transmit(&hspi2, &dat, 1, 10);//发送
}


/******************************************************************************
	  函数说明：LCD写入数据
	  入口数据：dat 写入的数据
	  返回值：  无
******************************************************************************/
void LCD_WR_DATA(uint16_t dat)
{
	LCD_Writ_Bus(dat >> 8);
	LCD_Writ_Bus(dat);
}


/******************************************************************************
	  函数说明：LCD写入命令
	  入口数据：dat 写入的命令
	  返回值：  无
******************************************************************************/
void LCD_WR_REG(uint8_t dat)
{
	LCD_DC_Clr();//写命令
	// LCD_Writ_Bus(dat);
	while (HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_RESET);//检查接收标志位
	HAL_SPI_Transmit(&hspi2, &dat, 1, 10);//发送
	LCD_DC_Set();//写数据
}


/******************************************************************************
	  函数说明：设置起始和结束地址
	  入口数据：x1,x2 设置列的起始和结束地址
				y1,y2 设置行的起始和结束地址
	  返回值：  无
******************************************************************************/
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	if (USE_HORIZONTAL == 0)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1 + 52);
		LCD_WR_DATA(x2 + 52);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1 + 40);
		LCD_WR_DATA(y2 + 40);
		LCD_WR_REG(0x2c);//储存器写
	}
	else if (USE_HORIZONTAL == 1)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1 + 53);
		LCD_WR_DATA(x2 + 53);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1 + 40);
		LCD_WR_DATA(y2 + 40);
		LCD_WR_REG(0x2c);//储存器写
	}
	else if (USE_HORIZONTAL == 2)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1 + 40);
		LCD_WR_DATA(x2 + 40);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1 + 53);
		LCD_WR_DATA(y2 + 53);
		LCD_WR_REG(0x2c);//储存器写
	}
	else
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1 + 40);
		LCD_WR_DATA(x2 + 40);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1 + 52);
		LCD_WR_DATA(y2 + 52);
		LCD_WR_REG(0x2c);//储存器写
	}
}

void LCD_Init(void)
{
	// LCD_GPIO_Init();//初始化GPIO
	// SPI1_Init();    //初始化SPI1
	// LCD_RES_Clr();  //复位
	// delay(100);
	// LCD_RES_Set();
	// delay(100);

	// LCD_BLK_Set();//打开背光
	//delay(100);

	LCD_WR_REG(0x11);
	delay(120);
	LCD_WR_REG(0x36);
	if (USE_HORIZONTAL == 0)LCD_WR_DATA8_NO_DMA(0x00);
	else if (USE_HORIZONTAL == 1)LCD_WR_DATA8_NO_DMA(0xC0);
	else if (USE_HORIZONTAL == 2)LCD_WR_DATA8_NO_DMA(0x70);
	else LCD_WR_DATA8_NO_DMA(0xA0);

	LCD_WR_REG(0x3A);
	LCD_WR_DATA8_NO_DMA(0x05);

	LCD_WR_REG(0xB2);
	LCD_WR_DATA8_NO_DMA(0x0C);
	LCD_WR_DATA8_NO_DMA(0x0C);
	LCD_WR_DATA8_NO_DMA(0x00);
	LCD_WR_DATA8_NO_DMA(0x33);
	LCD_WR_DATA8_NO_DMA(0x33);

	LCD_WR_REG(0xB7);
	LCD_WR_DATA8_NO_DMA(0x35);

	LCD_WR_REG(0xBB);
	LCD_WR_DATA8_NO_DMA(0x19);

	LCD_WR_REG(0xC0);
	LCD_WR_DATA8_NO_DMA(0x2C);

	LCD_WR_REG(0xC2);
	LCD_WR_DATA8_NO_DMA(0x01);

	LCD_WR_REG(0xC3);
	LCD_WR_DATA8_NO_DMA(0x12);

	LCD_WR_REG(0xC4);
	LCD_WR_DATA8_NO_DMA(0x20);

	LCD_WR_REG(0xC6);
	LCD_WR_DATA8_NO_DMA(0x0F);

	LCD_WR_REG(0xD0);
	LCD_WR_DATA8_NO_DMA(0xA4);
	LCD_WR_DATA8_NO_DMA(0xA1);

	LCD_WR_REG(0xE0);
	LCD_WR_DATA8_NO_DMA(0xD0);
	LCD_WR_DATA8_NO_DMA(0x04);
	LCD_WR_DATA8_NO_DMA(0x0D);
	LCD_WR_DATA8_NO_DMA(0x11);
	LCD_WR_DATA8_NO_DMA(0x13);
	LCD_WR_DATA8_NO_DMA(0x2B);
	LCD_WR_DATA8_NO_DMA(0x3F);
	LCD_WR_DATA8_NO_DMA(0x54);
	LCD_WR_DATA8_NO_DMA(0x4C);
	LCD_WR_DATA8_NO_DMA(0x18);
	LCD_WR_DATA8_NO_DMA(0x0D);
	LCD_WR_DATA8_NO_DMA(0x0B);
	LCD_WR_DATA8_NO_DMA(0x1F);
	LCD_WR_DATA8_NO_DMA(0x23);

	LCD_WR_REG(0xE1);
	LCD_WR_DATA8_NO_DMA(0xD0);
	LCD_WR_DATA8_NO_DMA(0x04);
	LCD_WR_DATA8_NO_DMA(0x0C);
	LCD_WR_DATA8_NO_DMA(0x11);
	LCD_WR_DATA8_NO_DMA(0x13);
	LCD_WR_DATA8_NO_DMA(0x2C);
	LCD_WR_DATA8_NO_DMA(0x3F);
	LCD_WR_DATA8_NO_DMA(0x44);
	LCD_WR_DATA8_NO_DMA(0x51);
	LCD_WR_DATA8_NO_DMA(0x2F);
	LCD_WR_DATA8_NO_DMA(0x1F);
	LCD_WR_DATA8_NO_DMA(0x1F);
	LCD_WR_DATA8_NO_DMA(0x20);
	LCD_WR_DATA8_NO_DMA(0x23);

	LCD_WR_REG(0x21);

  	LCD_Clear();
	LCD_WR_REG(0x29);
}

uint8_t LCD_ReadId(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	uint8_t value = 0;
	uint8_t i = 0;
	extern SPI_HandleTypeDef hspi2;

	LCD_DC_Clr();//DC Low to send command
	LCD_WR_DATA8_NO_DMA(0x0b);//write read register id

    GPIO_InitStruct.Pin = LCD_SPI_MOSI_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);  //SDA Input with pull-up
    // GPIO_InitStruct.Pin = LCD_SPI_SCK_Pin;
	// GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	// HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);  //SDA Input with pull-up

	// LCD_DC_Set();//DC High to receive data

	// HAL_SPI_Receive(&hspi2, &value, 1, 10);
	for (i = 0;i < 8;i++) {
		LCD_SCLK_Clr();
		__NOP();
		value <<= 1;
		if (HAL_GPIO_ReadPin(LCD_SPI_MOSI_GPIO_Port, LCD_SPI_MOSI_Pin))//read SDA once
		{
			value |= 0x01;
		}
		LCD_SCLK_Set();//Clock high to read data
		__NOP();
	}

	LCD_DC_Set();//DC High to receive data
    GPIO_InitStruct.Pin = LCD_SPI_SCK_Pin|LCD_SPI_MOSI_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);  //Reconfigure SDA Output

	LCD_Init();
    APP_LCD_main_init();
	return value;
}








