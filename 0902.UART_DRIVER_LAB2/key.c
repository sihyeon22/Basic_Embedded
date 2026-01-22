#include "device_driver.h"

void Key_EXT_Poll_Init(void)
{
	Macro_Set_Bit(RCC->APB2ENR, 2); // PA power on
	Macro_Write_Block(GPIOA->CRL, 0xf, 0x8, 3*4); // PA3 - input
	Macro_Set_Bit(GPIOA->ODR, 3); // PULL-UP
}

int Key_EXT_Get_Pressed(void)
{
	return Macro_Extract_Area(~GPIOA->IDR, 0x1, 3);
}

void Key_Poll_Init(void)
{
	Macro_Set_Bit(RCC->APB2ENR, 2);
	Macro_Write_Block(GPIOA->CRL, 0xf, 0x44, 24);
}


#define COUNT_FOR_CHAT 		20000

static int Key_Check_Input(void)
{
	return ~Macro_Extract_Area(GPIOB->IDR, 0x3, 6) & 0x3;
}

int Key_Get_Pressed(void)
{
	unsigned int i, k;

	for(;;)
	{
		k = Key_Check_Input();

		for(i=0; i<COUNT_FOR_CHAT; i++)
		{
			if(k != Key_Check_Input())
			{
				break;
			}
		}

		if(i == COUNT_FOR_CHAT) break;
	}

	return k;
}

void Key_Wait_Key_Released(void)
{
	while(Key_Get_Pressed());
}

int Key_Wait_Key_Pressed(void)
{
	int k;

	while((k = Key_Get_Pressed()) == 0);
	return k;
}
