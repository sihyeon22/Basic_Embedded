#include "device_driver.h"

static void Sys_Init(void)
{
	Clock_Init();
	LED_Init();
	Uart_Init(115200);
	Key_Poll_Init();
	Control_Init();

	SCB->VTOR = 0x08003000;
	SCB->SHCSR = 0;
}

int speed = 0;
int dir = 0;
extern volatile int Key_Value;

#define SPEED_BASE (0.5)

void cmd_process(char x)
{
	volatile int i;

	int c = x - '0';
	if ((c >= 1) && (c <= 9)) speed = c;
	else
	{
		switch (x)
		{
			case 'F':
				dir = 1;
				break;
			case 'R':
				dir = -1;
				break;
			case 'S':
				dir = 0;
				break;
			default:
				return;
		}
	}
	
	for (i = 0; i < 10000; i++);
	TIM2_Motor_Run(dir, SPEED_BASE + 0.05 * speed);
	Uart1_Printf("%d, %d\n", dir, speed);
}

void key_process(int key)
{
	volatile int i;
	
	if (key)
	{
		if (key == 1) dir *= -1;
		if (key == 2) dir = 0;
	}

	for (i = 0; i < 10000; i++);
	TIM2_Motor_Run(dir, SPEED_BASE + 0.05 * speed);
	Uart1_Printf("%d, %d\n", dir, speed);
}

void Main(void)
{
	Sys_Init();
	TIM2_Motor_Out_Init();
	TIM2_Motor_Out_Freq_Generation(5000, 0.5);
	Key_ISR_Enable(1);

	char x;

	for (;;)
	{
		x = Uart1_Get_Pressed();

		if (x != 0)
		{
			Uart1_Printf("%c\n", x);
			cmd_process(x);
		}

		if (Key_Value)
		{
			key_process(Key_Value);
			Key_Value = 0;
		}
	}
}


#if 0
void Main(void)
{
	Sys_Init();
	Uart1_Printf("EXTI KEY ISR Test\n");

	Key_ISR_Enable(1);

	for(;;)
	{
		if (Key_Value)
		{
			Uart1_Printf("KEY=%d\n", Key_Value);
			Key_Value = 0;
		}

		LED_Display(1);
		TIM2_Delay(500);
		LED_Display(2);
		TIM2_Delay(500);
	}
}
#endif