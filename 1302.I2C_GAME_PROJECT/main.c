#include "device_driver.h"

static void Sys_Init(void)
{
	Clock_Init();
	LED_Init();
	Key_Poll_Init();
	Uart_Init(115200);

	SCB->VTOR = 0x08003000;
	SCB->SHCSR = 0;
}

extern volatile int Key_Value;
extern volatile int Uart1_Rx_In;
extern volatile int Uart1_Rx_Data;
extern volatile int TIM4_Expired;

void Main(void)
{
	// int d = 1;
	volatile int i, j, data;
	volatile int p1_score = 0;
	volatile int p2_score = 0;

	Sys_Init();
	Uart1_Printf("MINI PING-PONG\n");

	Key_ISR_Enable(1);
	// Uart1_RX_Interrupt_Enable(1);
	// TIM4_Repeat_Interrupt_Enable(1, 200);

	I2C_SC16IS752_Init(5000);
	I2C_SC16IS752_Config_GPIO(0xFF);

	for(;;)
	{
		for(i = 0; i < 8; i++)
		{
			if (Key_Value)
			{
				if ((Key_Value != 2) || (i != 7))
				{
					p1_score++;
					Uart1_Printf("<Score>\n");
					Uart1_Printf("%d(Player1) : %d(Player2)\n", p1_score, p2_score);
				}

				Key_Value = 0;
			}
			data = ~(1u<<i);
			I2C_SC16IS752_Write_GPIO(data);
			// Uart1_Printf("LED[%d] ON, Value = 0x%.2X\n", i, (unsigned char)data);
			for(j = 0; j < 1000000; j++);
		}
		for(i = 7; i >= 0; i--)
		{
			if (Key_Value)
			{
				if ((Key_Value != 1) || (i != 0))
				{
					p2_score++;
					Uart1_Printf("<Score>\n");
					Uart1_Printf("%d(Player1) : %d(Player2)\n", p1_score, p2_score);
				}

				Key_Value = 0;
			}
			data = ~(1u<<i);
			I2C_SC16IS752_Write_GPIO(data);
			// Uart1_Printf("LED[%d] ON, Value = 0x%.2X\n", i, (unsigned char)data);
			for(j = 0; j < 1000000; j++);
		}

	    // if(Uart1_Rx_In)
	    // {
		// 	Uart1_Printf("RX Data=%c\n", Uart1_Rx_Data);
		// 	Uart1_Rx_In = 0;
	    // }

	    // if(TIM4_Expired)
	    // {
		// 	LED_Display(d^=0x3);
		// 	TIM4_Expired = 0;
	    // }
	}
}
