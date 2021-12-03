#include "dma.h"

static inline void ENABLE_PERIPHERAL_CLOCK(void){
	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_DMA2EN);
	asm("NOP");
}

void DMA_Init(void)
{
	ENABLE_PERIPHERAL_CLOCK();

	NVIC_SetPriority(DMA2_Stream0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
	NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}
