#include "adc.h"
#include "math.h"

#define KELVIN_TO_CELSIUM(K) (K - 273.15f)

/* (1/T) = (1/T0)+(1/B)ln(R/R0) */

static const char LogTable256[256] =
{
#define LT(n) n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n
    -1, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
    LT(4), LT(5), LT(5), LT(6), LT(6), LT(6), LT(6),
    LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7)
};

static struct {
	const uint16_t B;
	const uint16_t R0;
	const uint16_t Rp; /* Pull up resistance */
	const uint16_t adcResolution;
	const float T0;
}thermometrContext = {
	.B = 3950,
	.R0 = 10000,
	.Rp = 10000,
	.adcResolution = (1<<ADC_RESOLUTION) - 1,
	.T0 = 298.15f,
};

__attribute__((weak)) float ln(float v)
{
	int log2;
	int x = *(const int *) &v;

	log2 = x >> 23;

	if (log2)
	{
		log2 -= 127;
	}
	else
	{
		register unsigned int t;
		if ((t = x >> 16))
		{
			log2 = LogTable256[t] - 133;
		}
		else
		{
			log2 = (t = x >> 8) ? LogTable256[t] - 141 : LogTable256[x] - 149;
		}
	}
	return 0.6931f * log2;
}

float thermometr_GetTemperature (void)
{
	uint32_t adcVal = ADC1_GetLastValue();
	float R = (float)thermometrContext.Rp / ((float)thermometrContext.adcResolution / adcVal - 1.f);
	float T = R/thermometrContext.R0;
	T = ln(T);
	T /= thermometrContext.B;
	T += 1.f/thermometrContext.T0;
	T = 1.f/T;
	return KELVIN_TO_CELSIUM(T);
}
