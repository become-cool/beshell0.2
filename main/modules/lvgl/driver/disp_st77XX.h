#ifndef DISP_ST77XX_H_
#define DISP_ST77XX_H_

#include "driver/spi_master.h"


#define DIRECTION0		0
#define DIRECTION90		1
#define DIRECTION180		2
#define DIRECTION270		3



// 屏幕驱动

#define SCREEN_DRIVER_ST7789  1
#define SCREEN_DRIVER_ST7789V 2

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
// st7789:2, st7789v:0
#define SCREEN_SPI_MODE 0

typedef struct {
	uint8_t chip ;
	uint16_t _width;
	uint16_t _height;
	uint16_t _offsetx;
	uint16_t _offsety;
	int8_t _dc;
	int8_t _cs;
	spi_device_handle_t spi_dev;
} st77xx_dev_t;

bool st77xx_spi_init(st77xx_dev_t * dev, uint8_t spi, uint8_t cs, uint8_t dc, uint32_t frequency) ;
void st7789v_init(st77xx_dev_t * dev, int width, int height, int offsetx, int offsety, uint8_t MADCTL);
void st7789_init(st77xx_dev_t * dev, int width, int height, int offsetx, int offsety, uint8_t MADCTL);
// void st77xx_pixel(st77xx_dev_t * dev, uint16_t x, uint16_t y, uint16_t color);
void st77xx_draw_rect(st77xx_dev_t * dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t * colors) ;
void st77xx_fill_rect(st77xx_dev_t * dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) ;

#endif