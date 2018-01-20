#ifndef _ADC_H
#define	_ADC_H

#include <stdint.h>


#define ADC_CHANNEL_DAC  0b11110  // DAC1OUT
#define ADC_CHANNEL_SRC  0b00101  // RC1 (AN5)
#define ADC_CHANNEL_AMP  0b00111  // RC3 (AN7)


void      adc_init();
uint16_t  adc_read(uint8_t channel);


#endif
