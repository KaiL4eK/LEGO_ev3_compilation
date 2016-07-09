#include <stdlib.h>
#include <stdio.h>

#include <unistd.h> // usleep

#include <ev3_button.h>

int main ( void )
{
    bool    up = false, 
            down = false, 
            left = false, 
            right = false, 
            center = false, 
            escape = false;

    while (!escape)
    {
        up = Button_pressed( BUTTON_UP );
        down = Button_pressed( BUTTON_DOWN );
        left = Button_pressed( BUTTON_LEFT );
        right = Button_pressed( BUTTON_RIGHT );
        center = Button_pressed( BUTTON_CENTER );
        escape = Button_pressed( BUTTON_BACK );

        printf ( "up:%d down:%d left:%d right:%d center:%d esc:%d\n", 
                    up, down, left, right, center, escape );
        usleep ( 100000 );
    }
    return( 0 );
}
