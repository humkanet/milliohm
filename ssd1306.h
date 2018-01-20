#ifndef _SSD1306_H
#define _SSD1306_H

#include <stdint.h>
#include <stdbool.h>

#define SSD1306_ADDR  0x3C

#define SSD1306_WITH_FONT8x16
#define SSD1306_WITH_FONT16x32

typedef enum {
	FONT8x16,
	FONT12x24,
	FONT16x32,
} FONT_ENUM;


bool  ssd1306_init();
void  ssd1306_text(uint8_t x, uint8_t y, const char *text, FONT_ENUM font);
void  ssd1306_move(uint8_t x, uint8_t y);
void  ssd1306_clear(uint8_t val);

extern uint8_t  i2c_start(uint8_t addr);
extern void     i2c_stop();
extern void     i2c_write(uint8_t val);
extern void     i2c_write_data(uint8_t *data, uint8_t len);

#endif
