#include "device_driver.h"

static unsigned int DMA_STATUS[7] = {DMA_STATUS_READY,DMA_STATUS_READY,DMA_STATUS_READY,DMA_STATUS_READY,DMA_STATUS_READY,DMA_STATUS_READY,DMA_STATUS_READY};
static DMA_Channel_TypeDef * DMA1_CH[7] = {DMA1_Channel1,DMA1_Channel2,DMA1_Channel3,DMA1_Channel4,DMA1_Channel5,DMA1_Channel6,DMA1_Channel7};

void DMA1_Ch_Init(int ch, void* maddr, void* paddr, union _DMA_CCR_UNI ccr, unsigned int tr_cnt)
{
	Macro_Set_Bit(RCC->AHBENR,0);

	if(DMA_STATUS[ch - 1] != DMA_STATUS_READY) DMA1_Stop(ch);

	DMA1_CH[ch - 1]->CMAR = (unsigned int)maddr;
	DMA1_CH[ch - 1]->CPAR = (unsigned int)paddr;
	Macro_Write_Block(DMA1_CH[ch-1]->CCR, 0x7ff, ccr.ui_data>>4, 4);
	DMA1_CH[ch - 1]->CNDTR = tr_cnt;
}

void DMA1_Start(int ch)
{
	DMA_STATUS[ch - 1] = DMA_STATUS_RUN;
	Macro_Clear_Bit(DMA1_CH[ch - 1]->CCR, 0);
	Macro_Set_Bit(DMA1_CH[ch - 1]->CCR, 0);
}

void DMA1_Stop(int ch)
{
	DMA_STATUS[ch - 1] = DMA_STATUS_READY;
	Macro_Clear_Bit(DMA1_CH[ch - 1]->CCR, 0);
}

void DMA1_ISR_Enable(int ch, unsigned int tei, unsigned int hti, unsigned int tci)
{
	if(tei || hti || tci)
	{
		Macro_Set_Bit(RCC->AHBENR,0);
		DMA1_CH[ch - 1]->CCR |= (((tei)?(1):(0))<<3)|(((hti)?(1):(0))<<2)|(((tci)?(1):(0))<<1);
		DMA1->IFCR = 0xf<<(4*(ch - 1));
		NVIC_ClearPendingIRQ(10 + ch);
		NVIC_EnableIRQ(10 + ch);
	}
	else
	{
		Macro_Clear_Area(DMA1_CH[ch - 1]->CCR, 0x7, 1);
		NVIC_DisableIRQ(10 + ch);
	}
}

void DMA1_Set_Status(int ch, unsigned int status)
{
	DMA_STATUS[ch - 1] = status;
}

unsigned int DMA1_Get_Status(int ch)
{
	unsigned int tmp;

	tmp = DMA_STATUS[ch - 1];

	if(tmp == DMA_STATUS_COMPLETE) DMA_STATUS[ch - 1] = DMA_STATUS_READY;
	return tmp;
}
