#include <pic.h>
#include "button.h"

#define U16(x)  ((uint16_t) (x))


void button_init(BUTTON *btn)
{	
	unsigned char pin = (1<<btn->pin);
	switch (btn->port){
#ifdef TRISA
		case BUTTON_PORTA:
			TRISA  |= pin;
			WPUA   |= pin;
			ANSELA &= ~pin;
			break;
#endif
#ifdef TRISB
		case BUTTON_PORTB:
			TRISB  |= pin;
			WPUB   |= pin;
			ANSELB &= ~pin;
			break;
#endif
#ifdef TRISC
		case BUTTON_PORTC:
			TRISC  |= pin;
			WPUC   |= pin;
			ANSELC &= ~pin;
			break;
#endif
	}
	btn->stage = BUTTON_STAGE_IDLE;
}


void button_tick(BUTTON *btn, uint16_t ms)
{
	// Считываем значение пина
	uint8_t pin;
	switch (btn->port){
#ifdef TRISA
		case BUTTON_PORTA:
			pin = PORTA & (1<<btn->pin);
			break;
#endif
#ifdef TRISB
		case BUTTON_PORTB:
			pin = PORTB & (1<<btn->pin);
			break;
#endif
#ifdef TRISC
		case BUTTON_PORTC:
			pin = PORTC & (1<<btn->pin);
			break;
#endif
	}
	//
	if (btn->stage==BUTTON_STAGE_IDLE){
		// Кнопка нажата, проверяем дребезг
		if (!pin){
			btn->ms    = ms;
			btn->stage = BUTTON_STAGE_NOISE;
		}
	}
	// Проверяем дребезг
	else if (btn->stage==BUTTON_STAGE_NOISE){
		if (U16(ms-btn->ms)<30) return;
		if (!pin){
			if (btn->event) btn->event(btn, BUTTON_EVENT_DOWN);
			btn->stage = BUTTON_STAGE_DOWN;
		}
		else btn->stage = BUTTON_STAGE_IDLE;
	}
	// Ждем когда кнопку отпустят
	else if (btn->stage==BUTTON_STAGE_DOWN){
		// Кнопка отпущена
		if (pin){
			if (btn->event) btn->event(btn, BUTTON_EVENT_UP);
			btn->stage = BUTTON_STAGE_IDLE;
		}
	}
	else if (btn->stage==BUTTON_STAGE_RESET){
		if (pin) btn->stage = BUTTON_STAGE_IDLE;
	}
}


void button_reset(BUTTON *btn)
{
	btn->stage = BUTTON_STAGE_RESET;
}
