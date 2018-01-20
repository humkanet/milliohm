#ifndef _DAC_H
#define	_DAC_H

#include <stdint.h>


#define DAC_VREF    2500
#define DAC_MAX     1023


inline void  dac_init();
void         dac_set(uint16_t val);
void         dac_on();
inline void  dac_off();


#endif
