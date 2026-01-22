#include "device_driver.h"

#if 0

#define TIM2_MAX		0xFFFF
#define TIM2_TICK 		20 						// usec
#define TIM2_FREQ 		(1000000. / TIM2_TICK) 	// Hz
#define TIM2_PLS_1MS	(TIM2_FREQ / 1000.)		// pls

void TIM2_Stopwatch_Start(void)
{
	// TIM2 activate
	Macro_Set_Bit(RCC->APB1ENR, 0);

	// TIM2 CR1 설정: down count, one pulse
	TIM2->CR1 |= (0x0 << 7) | (0x18 << 0);

	// PSC 초기값 설정 => 20usec tick이 되도록 설계 (50KHz)
	TIM2->PSC = (int)((TIMXCLK / TIM2_FREQ) + 0.5) - 1;

	// ARR 초기값 설정 => 최대값 0xFFFF 설정
	TIM2->ARR = TIM2_MAX;

	// UG 이벤트 발생
	Macro_Set_Bit(TIM2->EGR, 0);

	// TIM2 start
	Macro_Set_Bit(TIM2->CR1, 0);
}

unsigned int TIM2_Stopwatch_Stop(void)
{
	unsigned int time;

	// TIM2 stop
	Macro_Clear_Bit(TIM2->CR1, 0);

	// CNT 초기 설정값 (0xffff)와 현재 CNT의 펄스수 차이를 구하고
	// 그 펄스수 하나가 20usec이므로 20을 곱한값을 time에 저장
	time = (TIM2->ARR - TIM2->CNT) * TIM2_TICK;

	// 계산된 time 값을 리턴(단위는 usec)
	return time;
}

#endif

#if 0

// my_code

void TIM2_Stopwatch_Start(void)
{
	Macro_Set_Bit(RCC->APB1ENR, 0); // TIM2 activate

	// TIM2 CR1 설정: down count, one pulse
	TIM2->CR1 |= (0x0 << 7) | (0x18 << 0);

	// PSC 초기값 설정 => 20usec tick이 되도록 설계 (50KHz)
	TIM2->PSC = (HCLK / (50 * 1000)) - 1;

	// ARR 초기값 설정 => 최대값 0xFFFF 설정
	TIM2->ARR = 0xFFFF;

	// UG 이벤트 발생
	Macro_Set_Bit(TIM2->EGR, 0);

	// TIM2 start
	Macro_Set_Bit(TIM2->CR1, 0);
}

unsigned int TIM2_Stopwatch_Stop(void)
{
	unsigned int time;

	// TIM2 stop
	Macro_Clear_Bit(TIM2->CR1, 0);

	// CNT 초기 설정값 (0xffff)와 현재 CNT의 펄스수 차이를 구하고
	// 그 펄스수 하나가 20usec이므로 20을 곱한값을 time에 저장
	time = (TIM2->ARR - TIM2->CNT) * 20;

	// 계산된 time 값을 리턴(단위는 usec)
	return time;
}

#endif

#if 0

void TIM2_Delay(int time)
{
	Macro_Set_Bit(RCC->APB1ENR, 0);

	// TIM2 CR1 설정: down count, one pulse
	TIM2->CR1 = (0x0 << 7) | (0x18 << 0);

	// PSC 초기값 설정 => 20usec tick이 되도록 설계 (50KHz)
	TIM2->PSC = (int)((TIMXCLK / TIM2_FREQ) + 0.5) - 1;

	// ARR 초기값 설정 => 요청한 time msec에 해당하는 초기값 설정
	TIM2->ARR = time * TIM2_PLS_1MS;

	// UG 이벤트 발생
	Macro_Set_Bit(TIM2->EGR, 0);

	// Update Interrupt Pending Clear
	// rc_w0: input 0 to clear
	Macro_Clear_Bit(TIM2->SR, 0);

	// TIM2 start
	Macro_Set_Bit(TIM2->CR1, 0);

	// Wait timeout
	while(!(Macro_Check_Bit_Set(TIM2->SR, 0)));

	// Stop and Power off
	Macro_Clear_Bit(TIM2->CR1, 0);
	Macro_Clear_Bit(TIM2->DIER, 0);
}

void TIM2_Delay2(int time)
{
	Macro_Set_Bit(RCC->APB1ENR, 0);

	// TIM2 CR1 설정: down count, one pulse
	TIM2->CR1 = (0x0 << 7) | (0x18 << 0);

	// PSC 초기값 설정 => 20usec tick이 되도록 설계 (50KHz)
	TIM2->PSC = (int)((TIMXCLK / TIM2_FREQ) + 0.5) - 1;

	unsigned int t = TIM2_PLS_1MS * time;
	int n = t / 0xFFFF;
	int m = t % 0xFFFF;
	int i;
	for (i = 0; i < n; i++)
	{
		// ARR 초기값 설정 => 요청한 time msec에 해당하는 초기값 설정
		TIM2->ARR = 0xFFFF;

		// UG 이벤트 발생
		Macro_Set_Bit(TIM2->EGR, 0);

		// Update Interrupt Pending Clear
		// rc_w0: input 0 to clear
		Macro_Clear_Bit(TIM2->SR, 0);

		// TIM2 start
		Macro_Set_Bit(TIM2->CR1, 0);

		// Wait timeout
		while(!(Macro_Check_Bit_Set(TIM2->SR, 0)));

		// Stop and Power off
		Macro_Clear_Bit(TIM2->CR1, 0);
	}

	// ARR 초기값 설정 => 요청한 time msec에 해당하는 초기값 설정
	TIM2->ARR = m;

	// UG 이벤트 발생
	Macro_Set_Bit(TIM2->EGR, 0);

	// Update Interrupt Pending Clear
	// rc_w0: input 0 to clear
	Macro_Clear_Bit(TIM2->SR, 0);

	// TIM2 start
	Macro_Set_Bit(TIM2->CR1, 0);

	// Wait timeout
	while(!(Macro_Check_Bit_Set(TIM2->SR, 0)));

	// Stop and Power off
	Macro_Clear_Bit(TIM2->CR1, 0);
}

#endif

#if 1

#define TIM4_MAX		0xFFFF
#define TIM4_TICK 		20 						// usec
#define TIM4_FREQ 		(1000000. / TIM4_TICK) 	// Hz
#define TIM4_PLS_1MS	(TIM4_FREQ / 1000.)		// pls

void TIM4_Repeat(int time)
{
	Macro_Set_Bit(RCC->APB1ENR, 2);

	// 기본 레지스터 설정
	//  1. TIM4 CR1: ARPE=0, down counter, repeat mode
	//  2. PSC(50KHz),  ARR(reload시 값) 설정
	//  3. UG 이벤트 발생
	TIM4->CR1 = (0 << 7) | (1 << 4) | (0 << 3) | (0 << 0);
	TIM4->PSC = (int)((TIMXCLK / TIM4_FREQ) + 0.5) - 1;
	TIM4->ARR = time * TIM4_PLS_1MS;
	Macro_Set_Bit(TIM4->EGR, 0);

	// 1. Update Interrupt Pending Clear
	// 2. TIM4 start
	Macro_Clear_Bit(TIM4->SR, 0);
	Macro_Set_Bit(TIM4->CR1, 0);
}

int TIM4_Check_Timeout(void)
{
	// 타이머가 timeout 이면 1 리턴, 아니면 0 리턴
	int flag = Macro_Check_Bit_Set(TIM4->SR, 0);

	if (flag) Macro_Clear_Bit(TIM4->SR, 0);
	return flag;
}

void TIM4_Stop(void)
{
	Macro_Clear_Bit(TIM4->CR1, 0);
	Macro_Clear_Bit(TIM4->DIER, 0);
	Macro_Clear_Bit(RCC->APB1ENR, 2);
}

void TIM4_Change_Value(int time)
{
	TIM4->ARR = 50 * time;
}

#endif