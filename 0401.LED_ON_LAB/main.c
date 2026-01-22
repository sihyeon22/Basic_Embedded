#include <device_driver.h>

// 여기에 사용자 임의의 define을 작성하시오
#define GPIOA_CRL (*(unsigned long *)(0x40010800))
#define GPIOA_ODR (*(unsigned long *)(0x4001080C))

void Main(void)
{
	  // 이 부분은 임의로 바꾸지 마시오
	  RCC_APB2ENR |= (1<<3);
	  RCC_APB2ENR |= (1<<2);

	  // GPA[2]을 출력 모드로 설정하시오
	  GPIOA_CRL = 0x600;

	  // GPA[2]에 LED는 ON 시키도록 설정하시오
	  GPIOA_ODR = 0x0;
}

#if 0
void Main(void)
{
	  // 이 부분은 임의로 바꾸지 마시오
	  RCC_APB2ENR |= (1<<3);

	  // GPB[9:8]을 출력 모드로 설정하시오
	  GPIOB_CRH = 0x66;

	  // GPB[9:8]에 LED0은 OFF, LED1은 ON 시키도록 설정하시오
	  GPIOB_ODR = 0x100;
}
#endif