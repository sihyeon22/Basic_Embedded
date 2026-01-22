#include "device_driver.h"

#define Key_On()		(Macro_Check_Bit_Clear(GPIOA->IDR, 3))
#define Key_Off()		(Macro_Check_Bit_Set(GPIOA->IDR, 3))
#define LED_Toggle()	(Macro_Invert_Bit(GPIOA->ODR, 2))

static void Sys_Init(void)
{
	Clock_Init();
	LED_Init();
	Uart_Init(115200);
}

void Main(void)
{
	Sys_Init();
	Uart_Printf("KEY Input Test #2\n");

	// PA3을 입력으로, ODR을 1번(pull-up register)으로 선언
	Macro_Write_Block(GPIOA->CRL, 0xff, 0x86, 8);
	Macro_Set_Bit(GPIOA->ODR, 2); // LED-OFF
	Macro_Set_Bit(GPIOA->ODR, 3); // PULL-UP

	unsigned int enable = 1;
	for(;;)
	{
		// KEY0가 눌릴때마다 LED0의 값을 Toggling
		if (enable && Key_On())
		{
			LED_Toggle();
			enable = 0;
		}
		else if (!enable && Key_Off())
		{
			enable = 1;
		}
	}

}

/* Key 인식2 */

#if 0

void Main(void)
{
	Sys_Init();
	Uart_Printf("KEY Input Test #1\n");

	// KEY[1:0], GPB[7:6]을 GPIO 입력으로 선언
	Macro_Write_Block(GPIOB->CRL, 0xff, 0x44, 24);

	for(;;)
	{
		// KEY0이 눌렸으면 LED0를 ON, 안 눌렸으면 OFF
		// KEY1이 눌렸으면 LED1를 ON, 안 눌렸으면 OFF
		unsigned int x = Macro_Extract_Area(GPIOB->IDR, 0x3 ,6);
		Macro_Write_Block(GPIOB->ODR, 0x3, x, 8);
	}
}

#endif

/* Key 인식1 */

#if 0

void Main(void)
{
	Sys_Init();
	Uart_Printf("KEY Input Test #1\n");

	// KEY[1:0], GPB[7:6]을 GPIO 입력으로 선언
	Macro_Write_Block(GPIOB->CRL, 0xff, 0x44, 24);

	for(;;)
	{
		// KEY0이 눌렸으면 LED0를 ON, 안 눌렸으면 OFF
		if (Macro_Check_Bit_Set(GPIOB->IDR, 6))
			Macro_Set_Bit(GPIOB->ODR, 8);
		else
			Macro_Clear_Bit(GPIOB->ODR, 8);

		// KEY1이 눌렸으면 LED1를 ON, 안 눌렸으면 OFF
		if (Macro_Check_Bit_Set(GPIOB->IDR, 7))
			Macro_Set_Bit(GPIOB->ODR, 9);
		else
			Macro_Clear_Bit(GPIOB->ODR, 9);
	}
}

#endif

/* Key에 의한 LED Toggling(edge-detect 방식) */

#if 0

void Main(void)
{
	Sys_Init();
	Uart_Printf("KEY Input Toggling #1\n");

	// KEY[1:0], GPB[7:6]을 GPIO 입력으로 선언
	Macro_Write_Block(GPIOB->CRL, 0xff, 0x44, 24);
	
	unsigned int prev = Macro_Check_Bit_Set(GPIOB->IDR, 6);
	for(;;)
	{
		// KEY0가 눌릴때마다 LED0의 값을 Toggling
		unsigned int curr = Macro_Check_Bit_Set(GPIOB->IDR, 6);
		if (prev && !curr)
		{
			Macro_Invert_Bit(GPIOB->ODR, 8);
		}
		prev = curr;
	}
}

#endif


/* Key에 의한 LED Toggling(inter-lock 방식) */

#if 0

void Main(void)
{
	Sys_Init();
	Uart_Printf("KEY Input Toggling #1\n");

	// KEY[1:0], GPB[7:6]을 GPIO 입력으로 선언
	Macro_Write_Block(GPIOB->CRL, 0xff, 0x44, 24);
	
	unsigned int enable = 1;
	for(;;)
	{
		// KEY0가 눌릴때마다 LED0의 값을 Toggling
		if (enable && Macro_Check_Bit_Clear(GPIOB->IDR, 6))
		{
			Macro_Invert_Bit(GPIOB->ODR, 8);
			enable = 0;
		}
		else if (!enable && Macro_Check_Bit_Set(GPIOB->IDR, 6))
		{
			enable = 1;
		}
	}
}

#endif