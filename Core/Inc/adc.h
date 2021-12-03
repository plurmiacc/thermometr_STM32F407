#ifndef __ADC_H__
#define __ADC_H__

#include "main.h"

#define ADC_RESOLUTION 12U

static inline void ADC1_Start(void){
	SET_BIT(ADC1->CR2, ADC_CR2_SWSTART);
}

void ADC1_Init(void);
uint32_t ADC1_GetLastValue(void);

#endif /* __ADC_H__ */

