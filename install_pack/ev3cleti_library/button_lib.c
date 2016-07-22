#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h> 
#include <unistd.h>
#include <linux/input.h>

#include <ev3_button.h>
#include <ev3_bool.h>

int __ev3_button_fd = -1;

short Status[6];

bool Button_pressed ( short button )
{
	char keys[96];
	
	if ( __ev3_button_fd < 0 ) {
		__ev3_button_fd = open( "/dev/input/by-path/platform-gpio-keys.0-event", O_RDONLY );
	}
	lseek( __ev3_button_fd, 0L, SEEK_SET );
	ioctl ( __ev3_button_fd, EVIOCGKEY(sizeof(keys)), &keys );

	switch ( button )
	{
		case BUTTON_LEFT:
			return (keys[KEY_LEFT >> 3] & (1 << (KEY_LEFT & 7)) ? false : true);

		case BUTTON_UP:
			return (keys[KEY_UP >> 3] & (1 << (KEY_UP & 7)) ? false : true);

		case BUTTON_RIGHT:
			return (keys[KEY_RIGHT >> 3] & (1 << (KEY_RIGHT & 7)) ? false : true);

		case BUTTON_DOWN:
			return (keys[KEY_DOWN >> 3] & (1 << (KEY_DOWN & 7)) ? false : true);

		case BUTTON_CENTER:
			return (keys[KEY_ENTER >> 3] & (1 << (KEY_ENTER & 7)) ? false : true);

		case BUTTON_BACK:
			return (keys[KEY_BACKSPACE >> 3] & (1 << (KEY_BACKSPACE & 7)) ? false : true);
	}
	return( false );
}

#define TS_DEFAULT 0
#define TS_PRESSED 1

bool Button_click( short button )
{

	if(Status[button] == TS_DEFAULT && Button_pressed(button))
    {
      Status[button]= TS_PRESSED;
      return (false);
    }
    if(Status[button] == TS_PRESSED && !Button_pressed(button))
    {
      Status[button] = TS_DEFAULT;
      return (true);
    }

    return (false);

}

int Button_free_resources ( void )
{
	if ( __ev3_button_fd != -1 ) {
		close( __ev3_button_fd );
	}
	return( 0 );
}

