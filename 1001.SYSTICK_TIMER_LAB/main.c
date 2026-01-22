#include "device_driver.h"

static void Sys_Init(void)
{
	Clock_Init();
	LED_Init();
	Uart_Init(115200);
	Key_Poll_Init();
}

#if 1

void Main(void)
{
	volatile int i, j;
	unsigned int cnt;

	Sys_Init();
	Uart1_Printf("SysTick 시간 측정 test\n");

	for(i=1; i<=4; i++)
	{
		SysTick_Run(1000);
		cnt = 0x40000 * i;
		for(j=0; j<cnt; j++);
		SysTick_Stop();
		Uart1_Printf("Delay Time = %u msec\n", (int)((SysTick_Get_Load_Time() - SysTick_Get_Time())*(8.*1000./HCLK)));
	}

	Uart1_Printf("SysTick TICK 발생 test\n");

	i = 0x3;
	SysTick_Run(1000);

	for(;;)
	{
		if(SysTick_Check_Timeout())
		{
			LED_Display(Macro_Invert_Area(i, 0x3, 0));
			Uart1_Printf(".");
		}
	}
}

#endif

#if 0

void Main(void)
{
	Sys_Init();
	Uart1_Printf("SysTick Test\n");

	SysTick->CTRL = 0x0; // 9MHz, TIMER OFF
	SysTick->LOAD = 0xFFFFFF; // Maximum value(24bit)
	SysTick->VAL = 0x0; // Initialize

	Macro_Set_Bit(SysTick->CTRL, 0); // Timer ON

	for (;;)
	{
		volatile int i;
		Uart1_Printf("0x%.8x\n", SysTick->VAL);
		if (SysTick->VAL == 0) break;
	}
}

#endif