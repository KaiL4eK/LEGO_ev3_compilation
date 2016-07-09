#ifndef EV3_BUTTON_H_
#define EV3_BUTTON_H_

#include <ev3_core.h>

typedef enum ev3_button_id_
{
	BUTTON_LEFT,
	BUTTON_UP,
	BUTTON_RIGHT,
	BUTTON_DOWN,
	BUTTON_CENTER,
	BUTTON_BACK
}ev3_button_id_t;

bool Button_pressed ( ev3_button_id_t button );
int Button_free_resources ( void );

#endif // EV3_BUTTON_H_