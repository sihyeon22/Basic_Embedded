#include "device_driver.h"

static void Sys_Init(void)
{
	Clock_Init();
	LED_Init();
	Key_Poll_Init();
	Uart_Init(115200);
}

void Main(void)
{
	volatile int i, j, data;

	Sys_Init();
	Uart_Printf("SPI BUS Interface Test\n");

	SPI_SC16IS752_Init(32); // PCLK2(36MHz)/32 = 1.125MHz (주파수 값이 아닌 분주비 전달)
	SPI_SC16IS752_Config_GPIO(0xFF);

	for(;;)
	{
		for(i=0;i<8;i++)
		{
			data = ~(1u<<i);
			SPI_SC16IS752_Write_GPIO(data);
			Uart1_Printf("LED[%d] ON, Value = 0x%.2X\n", i, (unsigned char)data);
			for(j=0;j<1000000;j++);
		}
	}
}
