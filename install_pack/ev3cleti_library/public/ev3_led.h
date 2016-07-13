#ifndef EV3_LED_H_
#define EV3_LED_H_

typedef enum led_side_ {
	LEFT,
	RIGHT
}Led_side_t;

typedef enum led_main_color_ {
	RED,
	GREEN
}Led_main_color_t;

void Led_set_color ( Led_side_t side, float red, float green );
void Led_set_flash ( Led_side_t side, Led_main_color_t mcolor, unsigned int on_ms, unsigned int off_ms );

#endif //EV3_LED_H_
