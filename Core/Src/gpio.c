#include "gpio.h"

static inline void ENABLE_PERIPHERAL_CLOCK(void){
	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
	asm("NOP");
}

void GPIO_Init(void)
{
	ENABLE_PERIPHERAL_CLOCK();
}
