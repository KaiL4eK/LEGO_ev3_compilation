#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <ev3_core.h>

int main ( void )
{
 
Lcd_clear();

    int Time = 0;
    uint8_t Flag = 0;
    do
    {

        Flag = Timer_count_from(5, &Time);
        Lcd_text(10,10 * (5 - Time),NORMAL,"Wait %d seconds...", Time);
        if(Flag)
            break;

    }while(true);

    Lcd_clear();

    Lcd_text(10,10, NORMAL, "Sleep 1 second...");
    Timer_sleep_until(1);
    Lcd_clear();

    do
    {

        Lcd_text(10,10,NORMAL, "%d", Timer_get_time());
        Lcd_text(10,20,NORMAL, "HOLD Left Button");
        Lcd_text(10,30,NORMAL, "to EXIT");
        Timer_sleep_until(1);
        Lcd_clear();

        if(Button_pressed(BUTTON_LEFT))
            break;

    }while(1);

    Lcd_clear();
    Lcd_release();
    Button_free_resources();
    return( 0 );
}

