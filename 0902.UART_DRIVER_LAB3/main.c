#include "device_driver.h"

static void Sys_Init(void)
{
	Clock_Init();
	LED_EXT_Init();
	Key_EXT_Poll_Init();
	Uart_Init(115200);
}

static void LED_Control(void)
{
	char x = Uart1_Get_Pressed();
	Uart_Printf("%c\n", x);
	if (x == '0') LED_EXT_Display(0);
	else if (x == '1') LED_EXT_Display(1);
}

static void KEY_Control(void)
{
	static int cnt = 0;
	static int key_check_en = 1;

	if (key_check_en && (Key_EXT_Get_Pressed() == 1))
	{
		cnt++;
		Uart_Printf("Pushed Count: %d\n", cnt);
		key_check_en = 0;
	}
	else if (!key_check_en && (Key_EXT_Get_Pressed() == 0))
	{
		key_check_en = 1;
	}
}

void Main(void)
{
	Sys_Init();
	Uart_Printf("MINI PROJECT(example)\n");

	for (;;)
	{
		LED_Control();
		KEY_Control();
	}
}

#if 0

// Key Test
void Main(void)
{
	Sys_Init();
	Uart_Printf("EXT KEY TEST1\n");

	for(;;)
	{
		int x = Key_EXT_Get_Pressed();
		Uart_Printf("%d\n", x);
	}
}

#endif

#if 0

// LED Test
void Main(void)
{
	volatile int i;

	Sys_Init();
	Uart_Printf("EXT LED TEST1\n");

	for(;;)
	{
		LED_EXT_Display(0x1);
		for(i=0; i<0x100000; i++);
		LED_EXT_Display(0x2);
		for(i=0; i<0x100000; i++);
	}
}

#endif