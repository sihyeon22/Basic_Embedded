#include "device_driver.h"

#if 1

#define Turn_Right() 	(Macro_Write_Block(GPIOA->ODR, 0x3, 0x2, 2))
#define Turn_Left() 	(Macro_Write_Block(GPIOA->ODR, 0x3, 0x1, 2))
#define Motor_Stop()	(Macro_Clear_Area(GPIOA->ODR, 0x3, 2))

static void Sys_Init(void)
{
	Uart_Init(115200);
	Clock_Init();
	LED_Init();
	Key_Poll_Init();
	Motor_Init();
}

void Main(void)
{
	Sys_Init();
	Uart_Printf("Motor Test\n");

	for(;;)
	{
		char x = Uart1_Get_Pressed();
		Uart_Printf("%c\n", x);
		switch (x)
		{
			case 'F':
				TIM2_Delay(1);
				Turn_Right();
				break;
			case 'R':
				TIM2_Delay(1);
				Turn_Left();
				break;
			case 'S':
				Motor_Stop();
				break;
			default:
				break;
		}
	}
}

#endif

#if 0

#define STOP    0x0
#define FWD     0x1
#define BWD     0x2

static void Sys_Init(void)
{
	Uart_Init(115200);
	Clock_Init();
	LED_Init();
	Key_Poll_Init();
	Motor_Init();
}

void Main(void)
{
	Sys_Init();
	Uart1_Printf("Motor Test\n");

	for(;;)
	{
		char x = Uart1_Get_Char(); // 'S' = 0, 'F' = 1, 'R' = 2
		Uart1_Printf("%c\n", x);

		if (x == 'F') Motor_Run(FWD);
		else if (x == 'R') Motor_Run(BWD);
		else if (x == 'S') Motor_Run(STOP);
		else Uart1_Printf("Please retry.\n");
	}
}

#endif