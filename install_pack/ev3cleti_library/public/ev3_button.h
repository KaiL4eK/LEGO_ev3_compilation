#ifndef EV3_BUTTON_H_
#define EV3_BUTTON_H_

#include <ev3_bool.h>

#define BUTTON_LEFT 0
#define BUTTON_UP 1
#define BUTTON_RIGHT 2
#define BUTTON_DOWN 3
#define BUTTON_CENTER 4
#define BUTTON_BACK 5

/*typedef enum ev3_button_id_
{
	BUTTON_LEFT,
	BUTTON_UP,
	BUTTON_RIGHT,
	BUTTON_DOWN,
	BUTTON_CENTER,
	BUTTON_BACK
}ev3_button_id_t;*/

bool Button_pressed ( short button );
bool Button_click( short button );

#endif // EV3_BUTTON_H_