#include <stdlib.h>
#include "ssd1306.h"

#define SSD1306_MODE_CMD   0x00
#define SSD1306_MODE_DATA  0x40

#ifdef SSD1306_WITH_FONT16x32
	#include "fonts/font16x32_less.h"
	#define _WITH_FONTS
#endif

#ifdef SSD1306_WITH_FONT12x24
	#include "fonts/font12x24.h"
	#define _WITH_FONTS
#endif

#ifdef SSD1306_WITH_FONT8x16
	#include "fonts/font8x16.h"
	#define _WITH_FONTS
#endif


const uint8_t ssd1306_init_sequence[] = {
	0xAE, // display off
	0xD5, // clock
	0x81, // upper nibble is rate, lower nibble is divisor
	0xA8, // mux ratio
	0x1F, // rtfm
	0xD3, // display offset
	0x00, // rtfm
	0x00,
	0x8D, // charge pump
	0x14, // enable
	0x20, // memory addr mode
	0x00, // horizontal
	0xA1, // segment remap
	0xA5, // display on
	0xA0, // !!! X-Mirror: 0xA0 !!!
	0xC0, // !!! Y-Mirror: 0xC0 !!! default: 0xC8
	0xDA, // com hardware cfg
	0x02, // alt com cfg
	0x81, // contrast aka current
	0x7F, // 128 is midpoint
	0xD9, // precharge
	0x11, // rtfm
	0xDB, // vcomh deselect level
	0x20, // rtfm
	0xA6, // non-inverted
	0xA4, // display scan on
	0xAF, // drivers on
};


bool ssd1306_start(uint8_t mode)
{
	// Device not found
	if (!i2c_start(SSD1306_ADDR<<1)){
		i2c_stop();
		return false;
	}
	// Write mode
	i2c_write(mode);
	// All OK
	return true;
}


bool ssd1306_init()
{
	// Initialize display
	if (!ssd1306_start(SSD1306_MODE_CMD)){
		return false;
	}
	i2c_write_data((uint8_t *) ssd1306_init_sequence,
		sizeof(ssd1306_init_sequence));
	i2c_stop();
	// All OK
	return true;
}


void ssd1306_move(uint8_t x, uint8_t y)
{
	if (ssd1306_start(SSD1306_MODE_CMD)){
		i2c_write(0xB0 + y);
		i2c_write(x & 0x0F);
		i2c_write(0x10 | (x>>4));
		i2c_stop();
	}
}


#ifdef _WITH_FONTS
void ssd1306_text(uint8_t x, uint8_t y, const char *text, FONT_ENUM font)
{
	// Выбираем шрифт
	const SSD1306_FONT      *cfg;
	const SSD1306_FONT_DATA *data;
	uint8_t                 size;
	switch (font){
#ifdef SSD1306_WITH_FONT8x16
		case FONT8x16:
			cfg  = &font8x16;
			data = &font8x16_data;
			break;
#endif
#ifdef SSD1306_WITH_FONT12x24
		case FONT12x24:
			cfg  = &font12x24;
			data = &font12x24_data;
			break;
#endif
#ifdef SSD1306_WITH_FONT16x32
		case FONT16x32:
			cfg  = &font16x32;
			data = &font16x32_data;
			break;
#endif
		default:
			return;
	}
	size = cfg->width*cfg->pages;
	// Отрисовываем текст
	for (uint8_t page=0; page<cfg->pages; page++){
		ssd1306_move(x, page+y);
		if (!ssd1306_start(SSD1306_MODE_DATA)) return;
		char *addr = (char *) text;
		while (*addr){
			// Проверяем символ
			char c = *addr++;
			if ((c<cfg->first) || (c>cfg->last)) c=cfg->invalid;
			// Отрисовка
			c -= cfg->first;
			i2c_write_data(data+c*size, cfg->width);
		}
		i2c_stop();
		data += cfg->width;
	}
}
#endif


void ssd1306_clear(uint8_t val)
{
	// Выбираем адрес 0, 0
	ssd1306_move(0, 0);
	if (ssd1306_start(SSD1306_MODE_DATA)){
		for (uint16_t n=0; n<128*4; n++){
			i2c_write(val);
		}
		i2c_stop();
	}
}
