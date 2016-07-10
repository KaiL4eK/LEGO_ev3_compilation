#ifndef EV3_LCD_H_
#define EV3_LCD_H_

typedef enum pixel_color_ {
	WHITE = 0,
	BLACK
}Pixel_color_t;

typedef enum figure_fill_type_ {
	FILL = 0,
	BORDER
}Figure_fill_type_t;

typedef enum font_size_ {
	LARGE,
	NORMAL,
	SMALL,
	TINY
}Font_size_t;

void Lcd_release ( void );
void Lcd_clear( void );
void Lcd_text ( int16_t x, int16_t y, const char *text, Font_size_t size );
void Lcd_draw_rectangle ( int16_t x, int16_t y, int16_t w, int16_t h, Pixel_color_t color, Figure_fill_type_t type );
void Lcd_draw_ellipse_filled ( int16_t x, int16_t y, int16_t rx, int16_t ry, Pixel_color_t color );
void Lcd_draw_ellipse_edged ( int16_t x, int16_t y, int16_t rx, int16_t ry, Pixel_color_t color );
void Lcd_draw_circle_filled ( int16_t x, int16_t y, int16_t r, Pixel_color_t color );
void Lcd_draw_circle_edged ( int16_t x, int16_t y, int16_t r, Pixel_color_t color );
void Lcd_draw_line( int16_t x0, int16_t y0, int16_t x1, int16_t y1, Pixel_color_t color );

#endif //EV3_LCD_H_
