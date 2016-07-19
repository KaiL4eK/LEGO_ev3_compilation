#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <ev3_core.h>

int main ( void )
{
   Lcd_clear();

    Lcd_text( 10, 80, LARGE, "Large" );
    Lcd_text( 100, 80, NORMAL, "Normal" );
    Lcd_text( 100, 90, SMALL, "Small" );
    Lcd_text( 100, 100, TINY, "Tiny" );

    Lcd_text( 60, 10, NORMAL, "Int: %d;", 123);
    Lcd_text( 60, 20, NORMAL, "Float: %f;", 1.23);

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

    Lcd_text( 10, 110, NORMAL, "Press button to quit" );
    getchar();    

    Lcd_release();
    return( 0 );
}
