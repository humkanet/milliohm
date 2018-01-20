#ifndef _BUTTON_H
#define _BUTTON_H

#include <stdint.h>


#define BUTTON_PORTA        0x00
#define BUTTON_PORTB        0x01
#define BUTTON_PORTC        0x02


#define BUTTON_EVENT_DOWN   0x01
#define BUTTON_EVENT_UP     0x02


#define BUTTON_STAGE_IDLE   0x00
#define BUTTON_STAGE_NOISE  0x01
#define BUTTON_STAGE_DOWN   0x02
#define BUTTON_STAGE_RESET  0x03


typedef struct _BUTTON BUTTON;
typedef void   (*BUTTON_EVENT)(BUTTON *btn, char e);

typedef struct _BUTTON {
	BUTTON_EVENT  event;
	unsigned      port : 2;
	unsigned      pin  : 3;
	unsigned      stage: 2;
	uint16_t      ms;
} BUTTON;

void  button_init(BUTTON *btn);
void  button_tick(BUTTON *btn, uint16_t ms);
void  button_reset(BUTTON *btn);

#endif
