#include "device_driver.h"

void Motor_Init(void)
{
	Macro_Set_Bit(RCC->APB2ENR, 2); // PA activate
	Macro_Write_Block(GPIOA->CRL, 0xFF, 0x22, 2*4);
    Macro_Clear_Area(GPIOA->ODR, 0x3, 2);
}


#if 1

// 아래는 해설 코드

#define STOP        0x0
#define FWD         0x1
#define BWD         0x2
#define MOT_DELAY   1

void Motor_Run(int cmd) // 0: stop, 1: fwd, 2: bwd
{
    static int dir = 0; // 0: stop, 1: fwd, 2: bwd
    static int mot[] = {STOP, FWD, BWD};

    if ((dir * cmd) == 2) TIM2_Delay(MOT_DELAY);
    Macro_Write_Block(GPIOA->ODR, 0x3, mot[cmd], 2);
}

#endif