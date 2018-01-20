#include <xc.h>
#include <stdint.h>
#include "amp.h"
#include "pre.h"

#define AMP_PORT  C
#define AMP_PIN   2


struct {
	union {
		struct {
			uint8_t init   : 1;
			uint8_t active : 1;
		};
		uint8_t flags;
	};
} amp;


void amp_init()
{
	// Флаги
	amp.flags = 0x00;
	// Настраиваем ОУ
	OPA1CON   = 0x00;
	OPA1NCHS  = 0x00;
	OPA1PCHS  = 0x02;
	OPA1ORS   = 0x00;
	// Отключаем выход ОУ
	LAT(AMP_PORT, AMP_PIN)  = 0;
	TRIS(AMP_PORT, AMP_PIN) = 0;
	// Флаги
	amp.init = 1;
}


void amp_on()
{
	if (amp.init && !amp.active){
		OPA1EN     = 1;
		__delay_us(10+5);
		amp.active = 1;
	}
}


void amp_off()
{
	if (amp.init && amp.active){
		OPA1EN     = 0;
		LAT(AMP_PORT, AMP_PIN)  = 0;
		TRIS(AMP_PORT, AMP_PIN) = 0;
		amp.active = 0;
	}
}
