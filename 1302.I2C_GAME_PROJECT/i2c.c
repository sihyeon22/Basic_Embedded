#include "device_driver.h"

/* freg = 5000 ~ 100000 */

#define SC16IS752_I2CADDR									0x9A
#define SC16IS752_I2CADDR_WR								(SC16IS752_I2CADDR|0x0)
#define SC16IS752_I2CADDR_RD								(SC16IS752_I2CADDR|0x1)

void I2C_SC16IS752_Init(unsigned int freq)
{
	unsigned int r;
	volatile int i;

	Macro_Set_Bit(RCC->APB1ENR, 21);

	Macro_Clear_Bit(RCC->APB1RSTR, 21);
	for(i=0; i<1000; i++);
	Macro_Set_Bit(RCC->APB1RSTR, 21);
	for(i=0; i<1000; i++);
	Macro_Clear_Bit(RCC->APB1RSTR, 21);

	
	Macro_Set_Bit(RCC->APB2ENR,0); // AFIO EN
	Macro_Set_Bit(RCC->APB2ENR, 3);
	Macro_Set_Bit(AFIO->MAPR, 1);
	Macro_Write_Block(GPIOB->CRH, 0xFF, 0xFF, 0);

	Macro_Write_Block(I2C1->CR2, 0x3f, PCLK1/1000000, 0);
	Macro_Clear_Bit(I2C1->CR1, 0);
	I2C1->TRISE = PCLK1/1000000+1;
	r = PCLK1/(freq*2);
	I2C1->CCR = ((r<4)?4:r);

	Macro_Clear_Bit(I2C1->CR1, 1);
	Macro_Set_Bit(I2C1->CR1, 0);
	Macro_Set_Bit(I2C1->CR1, 10);
}

void I2C_SC16IS752_Write_Reg(unsigned int addr, unsigned int data)
{
	while(Macro_Check_Bit_Set(I2C1->SR2, 1)); 					// Idle OK

	Macro_Set_Bit(I2C1->CR1, 8); 								// Start
	while(Macro_Check_Bit_Clear(I2C1->SR1, 0));					// Check Start

	I2C1->DR = SC16IS752_I2CADDR_WR ;							// Send WR Address
	while(Macro_Check_Bit_Clear(I2C1->SR1, 1));					// Check Address
	(void)I2C1->SR2;											// Clear ADDR flag by reading SR2

	I2C1->DR = addr<<3;											// Send Register Address
	while(Macro_Check_Bit_Clear(I2C1->SR1, 2));					// Check Byte Transfer Finished

	I2C1->DR = data;											// Send Data
	while(Macro_Check_Bit_Clear(I2C1->SR1, 2));					// Check Byte Transfer Finished

	Macro_Set_Bit(I2C1->CR1, 9); 								// Stop
	while(Macro_Check_Bit_Set(I2C1->CR1, 9));					// Check Stop(Auto Cleared)
}

void I2C_SC16IS752_Config_GPIO(unsigned int config)
{
	I2C_SC16IS752_Write_Reg(SC16IS752_IODIR, config);
}

void I2C_SC16IS752_Write_GPIO(unsigned int data)
{
	I2C_SC16IS752_Write_Reg(SC16IS752_IOSTATE, data);
}
