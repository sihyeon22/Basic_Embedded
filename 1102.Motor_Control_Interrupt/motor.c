#include "device_driver.h"

#define TIM2_FREQ 	  		(8000000.) 	      	    // Hz
#define TIM2_TICK	  		(1000000./TIM2_FREQ)	// usec
#define TIME2_PLS_OF_1ms  	(1000./TIM2_TICK)

void Control_Init(void)
{
    Macro_Set_Bit(RCC->APB1ENR, 0); // TIM2 activate
    Macro_Set_Bit(RCC->APB2ENR, 2); // PA activate
    Macro_Clear_Bit(GPIOA->ODR, 2);
    Macro_Clear_Bit(GPIOA->ODR, 3);
}

void FWD_Init(unsigned int freq)
{
    // PA2는 PWM, PA3는 GPIO Low로 초기화
    Macro_Write_Block(GPIOA->CRL, 0xFF, 0x2A, 2*4);
    
    TIM2->CCMR2 = (0x60 << 0);
    TIM2->CCER = (0 << 9) | (1 << 8);

    TIM2->CR1 = (1 << 4) | (0 << 3);
    TIM2->PSC = (int)((TIMXCLK / TIM2_FREQ) + 0.5) - 1;
    TIM2->ARR = (int)((TIM2_FREQ / freq) + 0.5);
}

void BWD_Init(unsigned int freq)
{
    // PA2는 GPIO, PA3는 PWM로 초기화
    Macro_Write_Block(GPIOA->CRL, 0xFF, 0xA2, 2*4);

    TIM2->CCMR2 = (0x60 << 8);
    TIM2->CCER = (0 << 13) | (1 << 12);

    TIM2->CR1 = (1 << 4) | (0 << 3);
    TIM2->PSC = (int)((TIMXCLK / TIM2_FREQ) + 0.5) - 1;
    TIM2->ARR = (int)((TIM2_FREQ / freq) + 0.5);
}

void PWM_Motor_Run(int dir, unsigned int freq, unsigned short duty)
{
    Macro_Clear_Bit(TIM2->CR1, 0);

    // FWD Run
    if (dir == 1)
    {
        FWD_Init(freq);
        TIM2->CCR3 = (TIM2->ARR * duty) / 100;

        Macro_Set_Bit(TIM2->EGR, 0);
        Macro_Set_Bit(TIM2->CR1, 0);
    }
    // BWD Run
    else if (dir == 0)
    {
        BWD_Init(freq);
        TIM2->CCR4 = (TIM2->ARR * duty) / 100;
    
        Macro_Set_Bit(TIM2->EGR, 0);
        Macro_Set_Bit(TIM2->CR1, 0);
    }
}

void Motor_Stop()
{
    Macro_Write_Block(GPIOA->CRL, 0xFF, 0x22, 2*4);
    Macro_Clear_Area(GPIOA->ODR, 0x3, 2);
}