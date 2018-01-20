#ifndef _MEASURE_H
#define _MEASURE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
	uint16_t  vsrc;
	uint16_t  vamp;
	uint16_t  r;
} MEASURE;


bool  measure_get(uint16_t dac, MEASURE *out);

#endif
