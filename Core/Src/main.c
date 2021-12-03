#include "main.h"
#include "syscfg.h"
#include "gpio.h"
#include "dma.h"
#include "adc.h"
#include "thermometr.h"

static float temperature;

static inline void InitALL(void){
	SysCfg_Init();
	GPIO_Init();
	DMA_Init();
	ADC1_Init();
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	InitALL();
	ADC1_Start();
	while(1)
	{
		temperature = thermometr_GetTemperature();
		UNUSED(temperature);
	}
}



/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {

  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{

}
#endif /* USE_FULL_ASSERT */

