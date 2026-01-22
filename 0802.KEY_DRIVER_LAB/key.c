#include "device_driver.h"

void Key_Poll_Init(void)
{
	Macro_Set_Bit(RCC->APB2ENR, 3);
	Macro_Write_Block(GPIOB->CRL, 0xff, 0x44, 24);
}

int Key_Get_Pressed(void)
{
	return Macro_Extract_Area(~GPIOB->IDR, 0x3, 6); // 반전시켜서 추출(추출 후 반전 아님!!!!)

#if 0
	const int r[] = {3, 2, 1, 0}; // Look-up table
	return r[x];
#endif

#if 0
	if (x == 0x3) return 0;
	else if (x == 0x2) return 1;
	else if (x == 0x1) return 2;
	else return 3;
#endif

#if 0
	switch(x)
	{
		case 0: return 3;
		case 1: return 2;
		case 2: return 1;
		default: return 0;
	}
#endif
}

void Key_Wait_Key_Released(void)
{
	for (;;)
	{
		if (Key_Get_Pressed() == 0) break;
	}
}

int Key_Wait_Key_Pressed(void)
{
	for (;;)
	{
		if (Key_Get_Pressed()) return Key_Get_Pressed();
	}
}
