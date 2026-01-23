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

#if 1

static int speed = 0;
static int dir = 0;
static int dir_back = 0;

#define SPEED_BASE	(0.5)

static void cmd_process(char x)
{
	int c = x - '0';

	if((c >= 1) && (c <= 9)) speed = c;

	else{
		switch(x)
		{
			case 'F' : dir_back = dir = 1; break;
			case 'R' : dir_back = dir = -1; break;
			case 'S' : dir = 0; break;
			default : return;
		}
	}

	TIM2_Motor_Run(dir, SPEED_BASE + 0.05 * speed);
	Uart1_Printf("%d, %d\n", dir, speed);
}

static void key_process(int key)
{
	if(key == 1)
	{
		dir = 0;
		Uart1_Printf("dir = %d\n", dir);
	}

	if(key == 2)
	{
		dir = dir_back;
		Uart1_Printf("dir_back = %d\n", dir);
	}

	TIM2_Motor_Run(dir, SPEED_BASE + 0.05 * speed);
}

extern volatile int Key_Value;

void Main(void)
{
	Sys_Init();
	TIM2_Motor_Out_Init();
	TIM2_Motor_Out_Freq_Generation(5000, 0.5);
	Key_ISR_Enable(1);

	char x;

	for(;;)
	{
		if((x = Uart1_Get_Pressed()) != 0) 
		{
			Uart1_Printf("%c\n", x);
			cmd_process(x);
		}

		if(Key_Value != 0)
		{
			key_process(Key_Value);
			Key_Value = 0;
		}
	}
}

#endif
