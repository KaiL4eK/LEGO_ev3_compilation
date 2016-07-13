#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <ev3_core.h>

int main ( void )
{
    Lcd_clear();

    Lcd_text( 10, 80, "Large", LARGE );
    Lcd_text( 100, 80, "Normal", NORMAL );
    Lcd_text( 100, 90, "Small", SMALL );
    Lcd_text( 100, 100, "Tiny", TINY );

    sleep( 1 );
    Lcd_draw_rectangle_filled( 10, 30, 10, 40, BLACK );

    sleep( 1 );
    Lcd_draw_rectangle_edged( 30, 30, 10, 40, BLACK );

    sleep( 1 );
    Lcd_draw_ellipse_filled( 55, 50, 10, 20, BLACK );

    sleep( 1 );
    Lcd_draw_ellipse_edged( 80, 50, 10, 20, BLACK );

    sleep( 1 );
    Lcd_draw_line( 100, 30, 120, 50, BLACK );
    Lcd_draw_line( 100, 50, 120, 30, BLACK );

    Lcd_text( 10, 110, "Press button to quit", NORMAL );
    getchar();    

    Lcd_release();
    return( 0 );
}
