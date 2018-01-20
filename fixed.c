#include "fixed.h"

int16_t fixed_int(int32_t value, uint8_t n)
{
	uint16_t e = 1;
	while(n--){
		e *= 10;
	}
	return value/e;
}


int16_t fixed_float(int32_t value, uint8_t n)
{
	uint16_t e = 1;
	while(n--){
		e *= 10;
	}
	value -= e*(value/e);
	if (value<0){
		value = -value;
	}
	return value;
}
