#include <string.h>
#include <xc.h>
#include "config.h"
#include "main.h"
#include "dac.h"
#include "adc.h"
#include "fosc.h"
#include "i2c.h"
#include "ssd1306.h"
#include "amp.h"
#include "button.h"
#include "clock.h"
#include "measure.h"
#include "str.h"
#include "fixed.h"

#define U32(x)  ((uint32_t) (x))
#define U16(x)  ((uint32_t) (x))


uint16_t  test();
void      btn1_event(BUTTON *btn, char e);

struct {
	uint16_t r2;
	uint16_t r2_prev;
} run = {0, 0};

BUTTON btn1 = { btn1_event, BUTTON_PORTC, 4 };


void interrupt isr()
{
	clock_isr();
}


void btn1_event(BUTTON *btn, char e)
{
	if (e!=BUTTON_EVENT_UP) return;
	// Выводим сообщение
	ssd1306_clear(0x00);
	ssd1306_text(0, 1, "Please wait ...", FONT8x16);
	// Измеряем значение
	run.r2_prev = test();
	// Выводим результат
	ssd1306_clear(0x00);
	if (run.r2_prev){
		char     line[9];
		uint16_t  r;
		memcpy(line, " xx.xxx", 8);
		if (run.r2){
			if (run.r2<=run.r2_prev){
				line[0] = '+';
				r = run.r2_prev - run.r2;
			}
			else{
				line[0] = '-';
				r = run.r2 - run.r2_prev;
			}
		}
		else{
			r = run.r2_prev;
		}
		itoa_pad(line+1, fixed_int(r, 3), 2, 0);
		itoa_pad(line+4, fixed_float(r, 3), 3, ITOA_PAD_ZERO);
		ssd1306_text(0, 0, line, FONT16x32);
	}
	else{
		ssd1306_text(0, 0, "  Result out    ", FONT8x16);
		ssd1306_text(0, 2, "      of range  ", FONT8x16);
	}
}


void setup()
{
	// Подтяжка
	WPUA   = 0x00;
	WPUC   = 0x00;
	nWPUEN = 0;
	// Инициализируем модули
	amp_init();
	dac_init();
	fosc_init();        
	adc_init();
	clock_init();
	// Инициализируем экран
	i2c_init();
	__delay_ms(200);
	ssd1306_init();
	// Инициализируем кнопки
	button_init(&btn1);
	// Разрешаем прерывания
	PEIE  = 1;
}


uint16_t test()
{
	// Включаем ЦАП
	dac_set(0);
	dac_on();
	// Включаем ОУ
	amp_on();
	// Подбираем максимальное опорное напряжение
	bool      ready = false;
	uint16_t  dac   = DAC_MAX - DAC_MARGIN;
	MEASURE   m;
	while (dac>=DAC_MARGIN){
		// Замеряем
		if (measure_get(dac, &m)){
			ready = true;
			break;
		}
		// Уменьшаем ЦАП
		dac --;
	}
	/*
	Напряжение на ЦАП довольно низкое, появляются шумы на ОУ
	Пробуем компенсировать их, измерив показания на соседнем
	положении ЦАП
	*/
	if (ready && (m.vsrc<150)){
		MEASURE m2;
		if (measure_get(dac-1, &m2)){
			m.vamp = (m.vamp+m2.vamp)>>1;
			m.vsrc = (m.vsrc+m2.vsrc)>>1;
			m.r    = (m.r+m2.r)>>1;
		}
	}
	// Отключаем ОУ
	amp_off();
	// Отключаем ЦАП
	dac_off();
	// Возвращаем значение
	return ready ? m.r : 0;
}


void main()
{
	di();   
	setup();
	ei();

	// Очищаем экран
	ssd1306_clear(0x00);
	ssd1306_text(0, 1, "Press ANY key...", FONT8x16);
    
	for(;;){
		uint16_t ms = clock_msec();
		// Обрабатываем кнопки
		button_tick(&btn1, ms);
		// Установка ноля
		if ((btn1.stage==BUTTON_STAGE_DOWN) && (U16(ms-btn1.ms)>1000)){
			ssd1306_clear(0x00);
			if (run.r2_prev){
				char buf[17];
				// Выводим сообщение
				memcpy(buf, "    xx.xxx Ohm  ", 17);
				itoa_pad(buf+4,   fixed_int(run.r2_prev, 3), 2, 0);
				itoa_pad(buf+7, fixed_float(run.r2_prev, 3), 3, ITOA_PAD_ZERO);
				ssd1306_text(0, 0, " Set <rel> to  ", FONT8x16);
				ssd1306_text(0, 2, buf, FONT8x16);
				run.r2 = run.r2_prev;
			}
			else{
				ssd1306_text(4, 1, "No measurements", FONT8x16);
			}
			button_reset(&btn1);
		}
		// Спим
		SLEEP();
	}
}
