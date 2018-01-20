#ifndef _FONTS_H
#define _FONTS_H

#include <stdint.h>

typedef uint8_t SSD1306_FONT_DATA;

typedef struct {
	uint8_t  width;
	uint8_t  pages;
	char     first;
	char     last;
	char     invalid;
} SSD1306_FONT;

#endif