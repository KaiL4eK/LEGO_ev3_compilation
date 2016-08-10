#ifndef EV3_LCD_H_
#define EV3_LCD_H_

#include <stdint.h>
#include <stddef.h>

typedef enum pixel_color_ {
	WHITE = 0,
	BLACK
}Pixel_color_t;

typedef enum font_size_ {
	LARGE,
	NORMAL,
	SMALL,
	TINY
}Font_size_t;

void Lcd_release ( void );
void Lcd_clear( void );
void Lcd_text ( int16_t x, int16_t y, Font_size_t size, const char *text, ...);
void Lcd_draw_rectangle_filled ( int16_t x, int16_t y, int16_t w, int16_t h, Pixel_color_t color );
void Lcd_draw_rectangle_edged ( int16_t x, int16_t y, int16_t w, int16_t h, Pixel_color_t color );
void Lcd_draw_ellipse_filled ( int16_t x, int16_t y, int16_t rx, int16_t ry, Pixel_color_t color );
void Lcd_draw_ellipse_edged ( int16_t x, int16_t y, int16_t rx, int16_t ry, Pixel_color_t color );
void Lcd_draw_circle_filled ( int16_t x, int16_t y, int16_t r, Pixel_color_t color );
void Lcd_draw_circle_edged ( int16_t x, int16_t y, int16_t r, Pixel_color_t color );
void Lcd_draw_line( int16_t x0, int16_t y0, int16_t x1, int16_t y1, Pixel_color_t color );
void Lcd_draw_pixel( int x, int y, Pixel_color_t color);

#endif //EV3_LCD_H_