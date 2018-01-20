#include "measure.h"
#include "adc.h"
#include "dac.h"
#include "amp.h"

#define U32(x)     ((uint32_t) (x))

#define ERROR_MAX   10
#define ERROR_MIN   4

typedef struct {
	uint16_t dac;
	uint8_t  error;
} ERROR;
const ERROR measure_errors[] = {
	{DAC_MAX/10, 12},
	{DAC_MAX/4, 8},
	{DAC_MAX/2, 6},
};
#define nERRORS  (sizeof(measure_errors)/sizeof(ERROR))


uint8_t measure_get_error(uint16_t dac)
{
	for (uint8_t n=0; n<nERRORS; n++){
		if (dac<measure_errors[n].dac) return measure_errors[n].error;
	}
	return ERROR_MIN;
}


bool measure_get(uint16_t dac, MEASURE *out)
{
	uint32_t vamp_sum;
	uint32_t vsrc_sum;
	uint8_t  vamp_err;
	// Выставляем значение ЦАП
	dac_set(dac);
	// Проверяем показания усилителя
	uint16_t vamp = adc_read(ADC_CHANNEL_AMP);
	if ((vamp<20) || (vamp>980)) return false;
	uint16_t vsrc = adc_read(ADC_CHANNEL_SRC);
	// Максимальная ошибка ОУ
	vamp_err = measure_get_error(dac);
	if      (vsrc<100) vamp_err = 12;
	else if (vsrc<150) vamp_err = 10;
	else if (vsrc<200) vamp_err = 8;
	else vamp_err = 6;
	// Усредняем результат
	uint16_t n    = 127;
	vamp_sum      = vamp;
	vsrc_sum      = vsrc;
	while(n--){
		// Расчитывае
		// Проверяем стабильность ИОН
		uint16_t vsrc2 = adc_read(ADC_CHANNEL_SRC);
		uint16_t dsrc  = vsrc2>vsrc ? vsrc2-vsrc : vsrc-vsrc2;
		if (dsrc>1) return false;
		vsrc_sum += vsrc2;
		// Считываем значение усилителя
		uint16_t vamp2 = adc_read(ADC_CHANNEL_AMP);
		uint16_t damp  = vamp2>vamp ? vamp2-vamp : vamp-vamp2;
		if (damp>vamp_err) return false;
		vamp_sum += vamp2;
	}
	// Расчитываем значения
	out->vsrc = (vsrc_sum>>7);
	out->vamp = (vamp_sum>>7);
	out->r    = (U32(AMP_R1)*250*out->vamp)/((U32(AMP_MULTIPLY)*out->vsrc - 100UL*out->vamp)>>2);
	// Все нормально
	return true;
}
