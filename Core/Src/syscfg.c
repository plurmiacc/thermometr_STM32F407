#include "syscfg.h"

static inline void ENABLE_SYSCFG(void){
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);
	asm("NOP");
}

static inline void ENABLE_PWR(void){
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN);
	asm("NOP");
}

static inline void ENABLE_FLASH(void){
	MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_0WS);
	while((READ_BIT(FLASH->ACR, FLASH_ACR_LATENCY)) != FLASH_ACR_LATENCY_0WS)
	{
		/* Wait till Flash is ready */
	}
}

static inline void ENABLE_VOLTAGE_SCALING(void){
	SET_BIT(PWR->CR, PWR_CR_VOS);
}

static inline void ENABLE_HSI(void){
	const uint32_t HSI_CALIBRATION_TRIMMING = 16;
	MODIFY_REG(RCC->CR, RCC_CR_HSITRIM, HSI_CALIBRATION_TRIMMING << RCC_CR_HSITRIM_Pos);
	SET_BIT(RCC->CR, RCC_CR_HSION);
	while(!(READ_BIT(RCC->CR, RCC_CR_HSIRDY) == (RCC_CR_HSIRDY)))
	{
		/* Wait till HSI is ready */
	}
}

static inline void ENABLE_SYSCLOCK(void){
	const uint32_t HCLKFrequency = 16000000;
	const uint32_t Ticks = 1000;

	MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1);
	MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV1);
	MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE2_DIV1);
	MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_HSI);

	while((READ_BIT(RCC->CFGR, RCC_CFGR_SWS)) != RCC_CFGR_SWS_HSI)
	{
		/* Wait till System clock is ready */
	}

	SysTick->LOAD  = (uint32_t)((HCLKFrequency / Ticks) - 1UL);
	SysTick->VAL   = 0UL;
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
	SystemCoreClock = HCLKFrequency;
}

inline void SysCfg_Init(void)
{
	ENABLE_SYSCFG();
	ENABLE_PWR();

	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));

	ENABLE_FLASH();
	ENABLE_VOLTAGE_SCALING();
	ENABLE_HSI();
	ENABLE_SYSCLOCK();
}
