#include <xc.h>
#include "adc.h"
#include "pre.h"


void adc_init()
{
	// Настраиваем АЦП
	ADCON0 = 0x00;
	ADCON1 = 0xF2;
	ADCON2 = 0x00;
	ADON   = 1;
}


uint16_t adc_read(uint8_t channel)
{
	// Выбираем канал
	ADCON0bits.CHS = channel;
	// Ждем пока стабилизируется
	__delay_us(500);
	// Замеряем
	GO_nDONE = 1;
	while (GO_nDONE);
	// Возвращаем результат
	return ADRES;
}
