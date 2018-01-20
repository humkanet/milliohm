#include <xc.h>
#include "clock.h"


#define CLOCK_MS   50

#define CLOCK_PR   (_LFINTOSC_FREQ*CLOCK_MS/1000)

#if CLOCK_PR<=256
	#define CLOCK_DIV   1
	#define CLOCK_CKPS  0b000
#elif (CLOCK_PR/2)<=256
	#define CLOCK_DIV   2
	#define CLOCK_CKPS  0b001
#elif (CLOCK_PR/4)<=256
	#define CLOCK_DIV   4
	#define CLOCK_CKPS  0b010
#elif (CLOCK_PR/8)<=256
	#define CLOCK_DIV   8
	#define CLOCK_CKPS  0b011
#elif (CLOCK_PR/16)<=256
	#define CLOCK_DIV   16
	#define CLOCK_CKPS  0b100
#elif (CLOCK_PR/32)<=256
	#define CLOCK_DIV   32
	#define CLOCK_CKPS  0b101
#elif (CLOCK_PR/64)<=256
	#define CLOCK_DIV   64
	#define CLOCK_CKPS  0b110
#elif (CLOCK_PR/128)<=256
	#define CLOCK_DIV   128
	#define CLOCK_CKPS  0b0111
#else
	#error "Invalid CLOCK_MS"
#endif


typedef union {
	struct {
		uint8_t  high;
		uint8_t  low;
	};
	uint16_t  val;
} U16_VAL;


volatile struct {
	uint8_t   sec;
	U16_VAL   msec;
	uint16_t  tmp;
	CLOCK_EVENT  event;
} clock;


inline void clock_isr()
{
	if (TMR2IF){
		TMR2IF = 0;
		clock.msec.val += CLOCK_MS;
		clock.tmp  += CLOCK_MS;
		if (clock.tmp>=1000){
			clock.tmp -= 1000;
			clock.sec  ++;
			if (clock.event) clock.event();
		}
	}
}


inline void clock_init()
{
	clock.sec   = 0;
	clock.tmp   = 0;
	clock.msec.val = 0;	
	clock.event = 0;
	// Настраиваем таймер
	T2CON    = 0x00 | (CLOCK_CKPS<<4);
	T2CLKCON = 0x04;
	T2HLT    = 0x00;
	T2RST    = 0x00;
	T2PR     = (CLOCK_PR/CLOCK_DIV)-1;
	T2TMR    = 0;
	// Настраиваем прерывания
	TMR2IF   = 0;
	TMR2IE   = 1;
	// Запускаем таймер
	T2ON     = 1;
}


inline uint8_t clock_sec()
{
	return clock.sec;
}


uint16_t clock_msec()
{
	U16_VAL ms = clock.msec;
	while(ms.high!=clock.msec.high){
		ms = clock.msec;
	}
	return ms.val;
}


inline void clock_event(CLOCK_EVENT event)
{
	clock.event = event;
}
