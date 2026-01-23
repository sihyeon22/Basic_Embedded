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

extern volatile int Key_Value;
extern volatile int Uart1_Rx_In;
extern volatile int Uart1_Rx_Data;
extern volatile int TIM4_Expired;

#if 1

#define P1_HIT_POS 0
#define P2_HIT_POS 7

static int pos = 3;
static int dir = 1; // 1: up, -1: down

static int P1_SCORE = 0;
static int P2_SCORE = 0;

void Ball_Draw(int p)
{

}

void Ball_Move(void)
{
	if ((pos == P1_HIT_POS) && (dir == -1))
	{
		P2_SCORE++;
		pos = P2_HIT_POS - 1;
		Uart1_Printf("%d : %d\n", P1_SCORE, P2_SCORE);
	}
	
	else if ((pos == P2_HIT_POS) && (dir == 1))
	{
		P1_SCORE++;
		pos = P1_HIT_POS + 1;
		Uart1_Printf("%d : %d\n", P1_SCORE, P2_SCORE);
	}

	else pos += dir;

	Ball_Draw(pos);
}

// I2C_SC16IS752_Write_GPIO(data);

void Main(void)
{
	Sys_Init();
	Uart1_Printf("LED Game\n");

	Key_ISR_Enable(1);
	Uart1_RX_Interrupt_Enable(1);
	TIM4_Repeat_Interrupt_Enable(1, 500);
	I2C_SC16IS752_Init(5000);
	I2C_SC16IS752_Config_GPIO(0xFF);

	for(;;)
	{
	    if(TIM4_Expired)
	    {
			Uart1_Printf("dir = %d, pos = %d\n", dir, pos);
			Ball_Move();
			TIM4_Expired = 0;
	    }

		if(Key_Value)
		{

			Key_Value = 0;
		}

	    if(Uart1_Rx_In)
	    {

			Uart1_Rx_In = 0;
	    }
	}
}

#endif

#if 0

void Main(void)
{
	volatile int i, j, data;

	Sys_Init();
	Uart_Printf("I2C BUS Interface Test\n");

	I2C_SC16IS752_Init(5000);
	I2C_SC16IS752_Config_GPIO(0xFF);

	for(;;)
	{
		for(i=0;i<8;i++)
		{
			data = ~(1u<<i);
			I2C_SC16IS752_Write_GPIO(data);
			Uart1_Printf("LED[%d] ON, Value = 0x%.2X\n", i, (unsigned char)data);
			for(j=0;j<1000000;j++);
		}
	}
}

#endif