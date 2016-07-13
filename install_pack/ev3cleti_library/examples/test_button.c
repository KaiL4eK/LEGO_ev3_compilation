#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> // usleep

#include <ev3_core.h>

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
        Lcd_clear();

        up = Button_pressed( BUTTON_UP );
        down = Button_pressed( BUTTON_DOWN );
        left = Button_pressed( BUTTON_LEFT );
        right = Button_pressed( BUTTON_RIGHT );
        center = Button_pressed( BUTTON_CENTER );
        escape = Button_pressed( BUTTON_BACK );

        if ( up )
            Lcd_draw_rectangle_filled( 80, 10, 30, 30, BLACK );
        else
            Lcd_draw_rectangle_edged( 80, 10, 30, 30, BLACK );
        
        if ( down )
            Lcd_draw_rectangle_filled( 80, 90, 30, 30, BLACK );
        else
            Lcd_draw_rectangle_edged( 80, 90, 30, 30, BLACK );
        
        if ( left )
            Lcd_draw_rectangle_filled( 40, 50, 30, 30, BLACK );
        else
            Lcd_draw_rectangle_edged( 40, 50, 30, 30, BLACK );
        
        if ( right )
            Lcd_draw_rectangle_filled( 120, 50, 30, 30, BLACK );
        else
            Lcd_draw_rectangle_edged( 120, 50, 30, 30, BLACK );
        
        if ( center )
            Lcd_draw_rectangle_filled( 80, 50, 30, 30, BLACK );
        else
            Lcd_draw_rectangle_edged( 80, 50, 30, 30, BLACK );

        usleep ( 100000 );
    }
    Lcd_release();
    return( 0 );
}
