#include "device_driver.h"

#if 1

static void Sys_Init(void)
{
	Clock_Init();
	LED_EXT_Init();
	Key_EXT_Poll_Init();
	Uart_Init(115200);
}

void Main(void)
{
	volatile int cnt = 0;
	unsigned int enable = 1;

	Sys_Init();
	Uart1_Printf("MINI PROJECT.ver2\n");

	for(;;)
	{
		// 키보드 입력에 의한 LED 제어
		char input = Uart1_Get_Pressed();
		if (input == '0')
		{
			LED_EXT_Display(0);
		}
		else if (input == '1')
		{
			LED_EXT_Display(1);
		}

		// 스위치 입력 횟수 출력
		unsigned int x = Key_EXT_Get_Pressed();
		if (enable && (x == 1))
		{
			cnt++;
			Uart1_Printf("Pushed count: %d\n", cnt);
			enable = 0;
		}
		else if (!enable && (x == 0))
		{
			enable = 1;
		}
	}
}

#endif

#if 0

** 이 코드의 문제점 **
Uart1_Get_Pressed() 함수는 한 번 값을 읽고 나면 버퍼를 비우기 때문에
if - else if ... 이런 식으로 사용할 때 함수를 직접 참조하게 되면 값을 제대로 읽지 못 할 수 있다.

다시 말해서, if (Uart1_Get_Pressed() == '0') 에서 값을 읽고 버퍼를 비우기 때문에
1이 들어오더라도 LED가 올바르게 동작하지 않는 경우가 발생함.

그래서 Uart1_Get_Pressed() 함수는 특정 변수에 값을 받아서 써야 함!!



#include "device_driver.h"

#define LED_ON()		(Macro_Clear_Bit(GPIOA->ODR, 2))
#define LED_OFF()		(Macro_Set_Bit(GPIOA->ODR, 2))
#define KEY_PUSHED()	(Macro_Check_Bit_Clear(GPIOA->IDR, 3))
#define KEY_RELEASED()	(Macro_Check_Bit_Set(GPIOA->IDR, 3))

static void Sys_Init(void)
{
	Clock_Init();
	LED_Init();
	Uart_Init(115200);
	Key_Poll_Init();
}

void Main(void)
{
	volatile int cnt = 0;
	unsigned int enable = 1;

	Sys_Init();
	Uart1_Printf("MINI PROJECT.ver1\n");

	Macro_Write_Block(GPIOA->CRL, 0xFF, 0x86, 8); // PA2를 출력(Open-drain)으로, PA3을 입력으로
	Macro_Set_Bit(GPIOA->ODR, 2); // LED OFF
	Macro_Set_Bit(GPIOA->ODR, 3); // PULL-UP

	for(;;)
	{
		// 키 입력에 의한 LED 제어
		if (Uart1_Get_Pressed() == '0')
		{
			LED_OFF();
		}
		else if (Uart1_Get_Pressed() == '1')
		{
			LED_ON();
		}

		// 스위치 입력 횟수 출력
		if (enable && KEY_PUSHED())
		{
			cnt++;
			Uart1_Printf("Pushed count: %d\n", cnt);
			enable = 0;
		}
		else if (!enable && KEY_RELEASED())
		{
			enable = 1;
		}
	}
}


#endif