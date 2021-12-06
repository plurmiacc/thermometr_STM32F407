#include "adc.h"

static volatile struct {
	volatile uint32_t adcval;
} adcContext = {
	.adcval = 0,
};

static inline void ENABLE_PERIPHERAL_CLOCK(void){
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC1EN);
	asm("NOP");
}

static inline void ENABLE_ADC1_GPIO(void){
	CLEAR_BIT(GPIOA->PUPDR,
			(GPIO_PUPDR_PUPDR0 << (POSITION_VAL(GPIO_BSRR_BS_0) * 2U))); /* No Pull */
	SET_BIT(GPIOA->MODER,
			(GPIO_MODER_MODER0 << (POSITION_VAL(GPIO_BSRR_BS_0) * 2U))); /* Analog */
}

static inline void ENABLE_ADC1_DMA(void){

	const DMA_TypeDef* DMAx = DMA2;
	const uint32_t StreamOffset = DMA2_Stream0_BASE - DMA2_BASE;
	volatile DMA_Stream_TypeDef* DMAStream = (DMA_Stream_TypeDef*)((uint32_t)((uint32_t)DMAx + (uint8_t)StreamOffset));

	CLEAR_BIT(DMAStream->CR, DMA_SxCR_CHSEL); /* 0 channel */

	CLEAR_BIT(DMAStream->CR, DMA_SxCR_DIR); /* From peripheral to memory */

	SET_BIT(DMAStream->CR, DMA_SxCR_PL); /* Very High */

	MODIFY_REG(DMAStream->CR, DMA_SxCR_PFCTRL, DMA_SxCR_CIRC); /* Circuit */

	CLEAR_BIT(DMAStream->CR, DMA_SxCR_PINC); /* No increment */

	CLEAR_BIT(DMAStream->CR, DMA_SxCR_MINC); /* No increment */

	MODIFY_REG(DMAStream->CR, DMA_SxCR_PSIZE, DMA_SxCR_PSIZE_0); /* Half Word */

	MODIFY_REG(DMAStream->CR, DMA_SxCR_MSIZE, DMA_SxCR_MSIZE_0); /* Half Word */

	CLEAR_BIT(DMAStream->FCR, DMA_SxFCR_DMDIS); /* Disable FiFo */

	WRITE_REG(DMAStream->PAR, (uint32_t)(&(ADC1->DR))); /* Source */

	WRITE_REG(DMAStream->M0AR, (uint32_t)(&(adcContext.adcval))); /* Destination */

	SET_BIT(DMAStream->CR, DMA_SxCR_EN); /* Enable Stream */
}

static inline void ENABLE_ADC1(void){
	CLEAR_BIT(ADC1->CR1, ADC_CR1_RES | ADC_CR1_SCAN); /* 12bit, Scan Disabled */
	CLEAR_BIT(ADC1->CR2, ADC_CR2_ALIGN); /* Right Align */

	CLEAR_BIT(ADC1->CR1, ADC_CR1_DISCEN | ADC_CR1_DISCNUM);
	MODIFY_REG(ADC1->CR2,
			ADC_CR2_EXTSEL | ADC_CR2_EXTEN,
			ADC_CR2_CONT | ADC_CR2_DDS | ADC_CR2_DMA);
	CLEAR_BIT(ADC1->SQR1, ADC_SQR1_L);

	SET_BIT(ADC1->CR2, ADC_CR2_EOCS);

	CLEAR_BIT(ADC123_COMMON->CCR,
			ADC_CCR_ADCPRE
			| ADC_CCR_MULTI
			| ADC_CCR_DMA
			| ADC_CCR_DDS
			| ADC_CCR_DELAY); /* Div2, Independent */

	SET_BIT(ADC1->CR2, ADC_CR2_ADON); /* Enable ADC1 */
}

void ADC1_Init(void)
{
	ENABLE_PERIPHERAL_CLOCK();
	ENABLE_ADC1_GPIO();
	ENABLE_ADC1_DMA();
	ENABLE_ADC1();
}

uint32_t ADC1_GetLastValue(void)
{
	return adcContext.adcval;
}
