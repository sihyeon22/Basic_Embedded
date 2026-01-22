#include "device_driver.h"
#define	printf	Uart1_Printf

static void Sys_Init(void)
{
	Clock_Init();
	LED_Init();
	Uart_Init(115200);
	Key_Poll_Init();

	SCB->VTOR = 0x08003000;
	SCB->SHCSR = 0;
}

/***********************************************************/
// [EX-19-4] UART에 문자열 인쇄
/***********************************************************/

#if 1

extern volatile int TIM4_Expired;
extern volatile int Uart1_Rx_In;
extern volatile int Uart1_Rx_Data;
extern volatile int Key_Value;

const char * str[] = {"[1] The DMA controller performs direct memory transfer by sharing the system bus with the Cortex™-M3 core.\n",
	"[2] The DMA request may stop the CPU access to the system bus for some bus cycles,\n",
	"[3] when the CPU and DMA are targeting the same destination (memory or peripheral).\n",
	"[4] The bus matrix implements round-robin scheduling, thus ensuring at least half of the system bus bandwidth (both to memory and peripheral) for the CPU.\n",
	"[5] After an event, the peripheral sends a request signal to the DMA Controller.\n",
	"[6] The DMA controller serves the request depending on the channel priorities.\n",
	"[7] As soon as the DMA Controller accesses the peripheral, an Acknowledge is sent to the peripheral by the DMA Controller.\n",
	"[8] The peripheral releases its request as soon as it gets the Acknowledge from the DMA Controller.\n",
	"[9] Once the request is deasserted by the peripheral, the DMA Controller release the Acknowledge.\n"};

void Main(void)
{
	unsigned int str_num = 0;

	Sys_Init();
	Uart1_Printf("USART String TX Test\n\n");
	
	Uart1_RX_Interrupt_Enable(1);
	TIM4_Repeat_Interrupt_Enable(1, 200);
	Key_ISR_Enable(1);

	for(;;)
	{
		if(str_num < (sizeof(str)/sizeof(str[0])))
		{
			Uart1_Printf("%s", str[str_num]);
			str_num++;
		}

		if(Uart1_Rx_In)
		{
			Uart1_Printf("RX Data=%c\n", Uart1_Rx_Data);
			Uart1_Rx_In = 0;
		}

		if(Key_Value)
		{
			Uart1_Printf("KEY=%d\n", Key_Value);
			Key_Value = 0;
		}

		if(TIM4_Expired)
		{
			static unsigned int led = 0x2;

			LED_Display(led = led^0x3);
			TIM4_Expired = 0;
		}
	}
}

#endif

/***********************************************************/
// [EX-19-5] H/W 트리거 DMA
/***********************************************************/

#if 0

#define CH	4

#include <string.h>

extern volatile int TIM4_Expired;
extern volatile int Uart1_Rx_In;
extern volatile int Uart1_Rx_Data;
extern volatile int Key_Value;
extern volatile int DMA1_CH_DONE[];

const char * str[] = {"[1] The DMA controller performs direct memory transfer by sharing the system bus with the Cortex™-M3 core.\n"
	"[2] The DMA request may stop the CPU access to the system bus for some bus cycles,\n"
	"[3] when the CPU and DMA are targeting the same destination (memory or peripheral).\n"
	"[4] The bus matrix implements round-robin scheduling, thus ensuring at least half of the system bus bandwidth (both to memory and peripheral) for the CPU.\n"
	"[5] After an event, the peripheral sends a request signal to the DMA Controller.\n"
	"[6] The DMA controller serves the request depending on the channel priorities.\n"
	"[7] As soon as the DMA Controller accesses the peripheral, an Acknowledge is sent to the peripheral by the DMA Controller.\n"
	"[8] The peripheral releases its request as soon as it gets the Acknowledge from the DMA Controller.\n"
	"[9] Once the request is deasserted by the peripheral, the DMA Controller release the Acknowledge.\n"};

void Main(void)
{
	unsigned int str_num=0;
	union _DMA_CCR_UNI ccr1;

	Sys_Init();
	Uart1_Printf("DMA Software Trigger Test\n");

	Uart1_RX_Interrupt_Enable(1);
	TIM4_Repeat_Interrupt_Enable(1, 200);
	Key_ISR_Enable(1);

	ccr1.ui_data = 0;

	ccr1.st_data.dir = DMA_DIR_M2P;
	ccr1.st_data.circ = DMA_CIRCULAR_DIS;
	ccr1.st_data.m2m = DMA_TRIGGER_HW;
	ccr1.st_data.minc = DMA_ADDR_INC;
	ccr1.st_data.pinc = DMA_ADDR_FIX;
	ccr1.st_data.msize = DMA_SIZE_8BIT;
	ccr1.st_data.psize = DMA_SIZE_8BIT;
	ccr1.st_data.pl = DMA_PL_VERYHIGH;

	Uart1_Printf("DMA(M2P) Test - H/W (USART) Trigger\n\n");

	Uart1_Wait_for_TX_Complete();
	DMA1_ISR_Enable(CH,0,0,1);
	Macro_Set_Bit(USART1->CR3, 7);

	for(;;)
	{
		if(DMA1_CH_DONE[CH] || (str_num == 0))
		{
			if(str_num < (sizeof(str)/sizeof(str[0])))
			{
				DMA1_Ch_Init(CH, (void *)str[str_num], (void*)(&USART1->DR), ccr1, strlen(str[str_num]));
				DMA1_Start(CH);
			}

			else
			{
				DMA1_Stop(CH);
			}

			str_num++;
			DMA1_CH_DONE[CH] = 0;
		}

		if(Uart1_Rx_In)
		{
			Uart1_Printf("RX Data=%c\n", Uart1_Rx_Data);
			Uart1_Rx_In = 0;
		}

		if(Key_Value)
		{
			Uart1_Printf("KEY=%d\n", Key_Value);
			Key_Value = 0;
		}

		if(TIM4_Expired)
		{
			static unsigned int led = 0x2;

			LED_Display(led = led^0x3);
			TIM4_Expired = 0;
		}
	}
}

#endif
