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

#if 1

#define P1_HIT_POS 	7
#define P2_HIT_POS 	0
#define TIME_GAP	50

static int p1_score = 0;
static int p2_score = 0;
volatile int pos = 0;
volatile int dir = 1;
volatile int data;

extern volatile int Key_Value;
extern volatile int Uart1_Rx_In;
extern volatile int Uart1_Rx_Data;
extern volatile int TIM4_Expired;

void Print_Score(void)
{
	Uart1_Printf("<Score>\n");
	Uart1_Printf("%d(Player1) : %d(Player2)\n\n", p1_score, p2_score);
}

void Ball_Display(void)
{
	data = ~(1u << pos);
	I2C_SC16IS752_Write_GPIO(data);

	pos += dir;

	if ((pos >= P1_HIT_POS) && (dir == 1))
	{
		pos = P1_HIT_POS;
		dir = -1;
	}
	else if ((pos <= P2_HIT_POS) && (dir == -1))
	{
		pos = P2_HIT_POS;
		dir = 1;
	}
}

void Hit_Check(int k)
{
	if (k == 1)
	{
		if (data != ~(1u << P1_HIT_POS))
		{
			p2_score++;
			Print_Score();
		}

		else Uart1_Printf("P1 HIT!\n\n");
	}

	else if (k == 2)
	{
		if (data != ~(1u << P2_HIT_POS))
		{
			p1_score++;
			Print_Score();
		}

		else Uart1_Printf("P2 HIT!\n\n");
	}
}

void Print_Result(void)
{
	Uart_Printf("------ GAME OVER ------\n\n");
	if (p1_score > p2_score)
	{
		Uart1_Printf("The Winner is P1!\n");
		Uart1_Printf("Congratulations!\n");
	}
	else if (p1_score < p2_score)
	{
		Uart1_Printf("The Winner is P2!\n");
		Uart1_Printf("Congratulations!\n");
	}
	else
	{
		Uart1_Printf("Draw\nPlease rematch\n");
	}
}

void Main(void)
{
	volatile int time = 500;

	Sys_Init();
	Uart1_Printf("MINI PING-PONG\n");

	Key_ISR_Enable(1);
	TIM4_Repeat_Interrupt_Enable(1, time);

	I2C_SC16IS752_Init(5000);
	I2C_SC16IS752_Config_GPIO(0xFF);

	for(;;)
	{
		if (Key_Value)
		{
			Hit_Check(Key_Value);
			Key_Value = 0;
			time -= TIME_GAP;
			Update_Time(time);
		}

		if (TIM4_Expired)
		{
			Ball_Display();
			TIM4_Expired = 0;
		}

		if (time == 0)
		{
			Key_ISR_Enable(0);
			TIM4_Repeat_Interrupt_Enable(0, time);
			Print_Result();
			break;
		}
	}
}

#endif