#include "device_driver.h"

char * _sbrk(int inc)
{
	extern unsigned char __ZI_LIMIT__;
	static char * heap = (char *)0;

	char * prevHeap;
	char * nextHeap;

	if(heap == (char *)0) heap = (char *)HEAP_BASE;

	prevHeap = heap;
	nextHeap = (char *)((((unsigned int)heap + inc) + 0x7) & ~0x7);

	if((unsigned int)nextHeap >= HEAP_LIMIT) return (char *)0;

	heap = nextHeap;
	return prevHeap;
}

void Clock_Init(void)
{
	RCC->CR = 0x1;

	Macro_Set_Bit(RCC->CR, 16);
	RCC->CFGR = ((9-2)<<18)|(0<<17)|(1<<16)|(0<<11)|(4<<8)|(0<<4)|(0<<0);

	Macro_Set_Bit(RCC->CR, 24);
	while(!Macro_Check_Bit_Set(RCC->CR, 25));

	Macro_Write_Block(FLASH->ACR, 0x3, 0x2, 0);
	Macro_Write_Block(RCC->CFGR, 0x3, 0x2, 0);
}
