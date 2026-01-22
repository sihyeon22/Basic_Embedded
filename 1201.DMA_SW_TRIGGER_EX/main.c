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
// [EX-19-1] SW에 의한 메모리 복사
/***********************************************************/

#if 1

extern volatile int TIM4_Expired;

#define NUM_OF_DATA 40

unsigned int src1_dat[NUM_OF_DATA];
unsigned int dst1_dat[NUM_OF_DATA];

void Main(void)
{
	unsigned int i=0;

	Sys_Init();
	Uart1_Printf("Memory Copy Test\n");

	TIM4_Repeat_Interrupt_Enable(1, 1000);

	for(i=0;i<NUM_OF_DATA;i++)
	{
		src1_dat[i] = i + 8;
		dst1_dat[i] = 0;
	}

	/* Copy from SRC to DST */
	for(i=0;i<NUM_OF_DATA;i++)
	{
		dst1_dat[i] = src1_dat[i];
	}

	/* Verify */
	for(i=0;i<NUM_OF_DATA;i++)
	{
		Uart1_Printf("dst1_dat[%d] = %d\n",i,dst1_dat[i]);
		dst1_dat[i] = 0;
	}

	for(;;)
	{
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
// [EX-19-2] DMA에 의한 메모리 복사
/***********************************************************/

#if 0

extern volatile int TIM4_Expired;
extern volatile int DMA1_CH_DONE[];

#define CH    		1
#define NUM_OF_DATA 40

unsigned int src1_dat[NUM_OF_DATA];
unsigned int dst1_dat[NUM_OF_DATA];

void Main(void)
{
	unsigned int i=0;
	union _DMA_CCR_UNI ccr1;

	Sys_Init();
	Uart1_Printf("DMA Software Trigger Test\n");

	Uart1_Wait_for_TX_Complete();
	TIM4_Repeat_Interrupt_Enable(1, 1000);

	for(i=0;i<NUM_OF_DATA;i++)
	{
		src1_dat[i] = i + 8;
		dst1_dat[i] = 0;
	}

	ccr1.ui_data = 0;
	ccr1.st_data.dir = DMA_DIR_M2P;
	ccr1.st_data.circ = DMA_CIRCULAR_DIS;
	ccr1.st_data.m2m = DMA_TRIGGER_SW;
	ccr1.st_data.minc = DMA_ADDR_INC;
	ccr1.st_data.pinc = DMA_ADDR_INC;
	ccr1.st_data.msize = DMA_SIZE_32BIT;
	ccr1.st_data.psize = DMA_SIZE_32BIT;
	ccr1.st_data.pl = DMA_PL_LOW;

	DMA1_Ch_Init(CH,src1_dat,dst1_dat,ccr1,NUM_OF_DATA);
	DMA1_ISR_Enable(CH,0,0,1);
	DMA1_Start(CH);

	for(;;)
	{
		if(DMA1_CH_DONE[CH])
		{
			Uart1_Printf("\nDMA-ch1 Complete!\n");

			for(i=0;i<NUM_OF_DATA;i++)
			{
				Uart1_Printf("dst1_dat[%d] = %d\n",i,dst1_dat[i]);
				dst1_dat[i] = 0;
			}

			DMA1_CH_DONE[CH] = 0;
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
// [EX-19-3] DMA 속성의 이해
/***********************************************************/

#if 0

#define CH1   1   /* 1 ~  7 */
#define CH2   7   /* 1 ~  7 */
#define NUM_OF_DATA 40

extern volatile int TIM4_Expired;
extern volatile int DMA1_CH_DONE[];

unsigned int src1_dat[NUM_OF_DATA];
unsigned int dst1_dat[NUM_OF_DATA];
unsigned int src2_dat[NUM_OF_DATA];
unsigned int dst2_dat[NUM_OF_DATA];

void Main(void)
{
	unsigned int i=0;

	union _DMA_CCR_UNI ccr1;
	union _DMA_CCR_UNI ccr2;

	Sys_Init();
	Uart1_Printf("Multiple DMA SW Trigger Test\n\n");

	Uart1_Wait_for_TX_Complete();
	TIM4_Repeat_Interrupt_Enable(1, 1000);

	for(i=0;i<NUM_OF_DATA;i++)
	{
		src1_dat[i] = i + 8;
		src2_dat[i] = 99 - i;
		dst1_dat[i] = 0;
		dst2_dat[i] = 0;
	}

	ccr1.ui_data = 0;
	ccr2.ui_data = 0;

	ccr1.st_data.dir = DMA_DIR_M2P;
	ccr1.st_data.circ = DMA_CIRCULAR_DIS;
	ccr1.st_data.m2m = DMA_TRIGGER_SW;
	ccr1.st_data.minc = DMA_ADDR_INC;
	ccr1.st_data.pinc = DMA_ADDR_INC;
	ccr1.st_data.msize = DMA_SIZE_32BIT;
	ccr1.st_data.psize = DMA_SIZE_32BIT;
	ccr1.st_data.pl = DMA_PL_LOW;

	ccr2.st_data.dir = DMA_DIR_P2M;
	ccr2.st_data.circ = DMA_CIRCULAR_EN;
	ccr2.st_data.m2m = DMA_TRIGGER_SW;
	ccr2.st_data.minc = DMA_ADDR_INC;
	ccr2.st_data.pinc = DMA_ADDR_INC;
	ccr2.st_data.msize = DMA_SIZE_32BIT;
	ccr2.st_data.psize = DMA_SIZE_32BIT;
	ccr2.st_data.pl = DMA_PL_VERYHIGH;

	DMA1_Ch_Init(CH1,src1_dat,dst1_dat,ccr1,NUM_OF_DATA);
	DMA1_Ch_Init(CH2,dst2_dat,src2_dat,ccr2,NUM_OF_DATA);

	DMA1_ISR_Enable(CH1,0,0,1);
	DMA1_ISR_Enable(CH2,0,0,1);

	DMA1_Start(CH1);
	DMA1_Start(CH2);

	for(;;)
	{
		if(DMA1_CH_DONE[CH1])
		{
			Uart1_Printf("\nDMA Ch-%d Complete!\n", CH1);

			for(i=0;i<NUM_OF_DATA;i++)
			{
				Uart1_Printf("dst1_dat[%d] = %d\n", i, dst1_dat[i]);
				dst1_dat[i] = 0;
			}

			DMA1_CH_DONE[CH1] = 0;
			Uart1_Printf("Restart Ch-%d\n", CH1);
			DMA1_Start(CH1);
		}

		if(DMA1_CH_DONE[CH2])
		{
			static int cnt = 0;

			Uart1_Printf("\nDMA Ch-%d Complete! [%d]\n", CH2, cnt++);

			for(i=0;i<NUM_OF_DATA;i++)
			{
				Uart1_Printf("dst2_dat[%d] = %d\n", i, dst2_dat[i]);
				dst2_dat[i] = 0;
			}

			DMA1_CH_DONE[CH2] = 0;
			Uart1_Printf("Finish Ch-%d\n", CH2);
			DMA1_Stop(CH2);
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
