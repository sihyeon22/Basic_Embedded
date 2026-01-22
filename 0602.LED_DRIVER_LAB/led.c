#include "device_driver.h"

void LED_Init(void)
{
	/* 다음 코드는 수정하지 마시오 */
	Macro_Set_Bit(RCC->APB2ENR, 3);

	Macro_Write_Block(GPIOB->CRH, 0xFF, 0x66, 0);
	Macro_Set_Area(GPIOB->ODR, 0x3, 8);
}

void LED_Display(unsigned int num)
{
	Macro_Write_Block(GPIOB->ODR, 0x3, (~num) & 0x3, 8);
}

void LED_All_On(void)
{
	Macro_Clear_Area(GPIOB->ODR, 0x3, 8);
}

void LED_All_Off(void)
{
	Macro_Set_Area(GPIOB->ODR, 0x3, 8);
}


#if 0

void LED_Init(void)
{
	/* 다음 코드는 수정하지 마시오 */
	Macro_Set_Bit(RCC->APB2ENR, 3);
	
	Macro_Write_Block(GPIOB->CRH, 0xFF, 0x66, 0);
	Macro_Set_Area(GPIOB->ODR, 0x3, 8);
}

void LED_Display(unsigned int num)
{
	num = Macro_Extract_Area(num, 0x3, 0);
	Macro_Write_Block(GPIOB->ODR, 0x3, ~num, 8);

	/*
	추출 후 반전하게 되면 필요한 비트를 제외하고 나머지 비트가 1로 설정됨.
	그러면 32bit 중 필요한 비트를 제외한 나머지 비트가 1로 세팅이 되어서 다른 부분에 영향을 줄 수 있음.

	그래서 추출하고 반전할 것이 아니라, 반전한 거에서 추출하는 게 중요!!!!

	num = Macro_Extract_Area(~num, 0x3, 0);
	Macro_Write_Block(GPIOB->ODR, 0x3, num, 8);

	++ 로컬 변수는 여러 개 사용해도 영향 없으므로 로컬 변수 자유롭게 사용하기
	*/
}

void LED_All_On(void)
{
	Macro_Clear_Area(GPIOB->ODR, 0x3, 8);
}

void LED_All_Off(void)
{
	Macro_Set_Area(GPIOB->ODR, 0x3, 8);
}

#endif