#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <ev3_core.h>

int main ( void )
{
    char buffer_volts[63],
         buffer_current[63];
    
    while ( 1 ) {
        Lcd_clear();

        sprintf( buffer_volts, "Voltage: %fV", Power_get_voltage() );
        Lcd_text( 5, 20, buffer_volts, NORMAL );

        sprintf( buffer_current, "Current: %fA", Power_get_current() );
        Lcd_text( 5, 40, buffer_current, NORMAL );

        Lcd_text( 5, 60, "Press Center to quit", NORMAL );

        usleep ( 100000 ); // 1s sleep
        if ( Button_pressed( BUTTON_CENTER ) ) {
            break;
        }
    } 
    Lcd_release();
    return( 0 );
}