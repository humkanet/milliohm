#include <xc.h>
#include "dac.h"
#include "pre.h"


#define VREF_PORT  A
#define VREF_PIN   1

inline void dac_init()
{
	// Настраиваем Vref+
	ANS(VREF_PORT, VREF_PIN)  = 1;
	TRIS(VREF_PORT, VREF_PIN) = 1;
	// Подключаем ЦАП к ИОН
	DAC1CON0 = 0x04;
	DAC1REF  = 0;
	DAC1LD   = 1;
}


void dac_set(uint16_t val)
{
	DAC1REF = val;
	DAC1LD  = 1;
	while(DAC1EN && DAC1LD);
	__delay_us(30+5);
}


void dac_on()
{
    DAC1EN = 1;
    __delay_us(30+5);
}


inline void dac_off()
{
    DAC1EN = 0;
}
