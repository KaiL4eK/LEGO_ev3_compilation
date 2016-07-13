#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> // usleep

#include <ev3_core.h>

int main ( void )
{
    bool escape = false;
    char buffer[63];

    while ( !escape ) {
        Lcd_clear();
        
        float value = UltraSonicSensor_read_cm( INPUT_1 );
        sprintf( buffer, "Val: %.1f", value );
        Lcd_text( 20, 20, buffer, LARGE );

        usleep( 500000 );
        escape = Button_pressed( BUTTON_BACK );
    }
    Lcd_release();
    return( 0 );
}