#include "device_driver.h"

static void Sys_Init(void)
{
	Clock_Init();
	LED_Init();
	Uart_Init(115200);
	Key_Poll_Init();

	SCB->VTOR = 0x08003000;
	SCB->SHCSR = 0;
}

volatile unsigned int key_value = 0;

void Main(void)
{
	Sys_Init();
	Uart1_Printf("EXTI Test\n");

	for(;;)
	{
		if (key_value != 0)
		{
			Uart1_Printf("KEY = %d\n", key_value);
			key_value = 0;
		}
		
		LED_Display(1);
		TIM2_Delay(500);
		LED_Display(2);
		TIM2_Delay(500);
	}
}