#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> // usleep

#include <ev3_core.h>

int main ( void )
{
    float   red = 0.0f,
            green = 1.0f;

    for ( red = 0.0f, green = 1.0f; 
          red < 1.0f && green > 0.0f; 
          red += 0.01f, green -= 0.01f ) {

        Led_set_color( RIGHT, red, green );
        Led_set_color( LEFT, 1-red, 1-green );

        usleep ( 50000 );
    }

    Led_set_flash( RIGHT, RED, 100, 100 );
    Led_set_flash( RIGHT, GREEN, 200, 200 );
    Led_set_flash( LEFT, RED, 800, 800 );
    Led_set_flash( LEFT, GREEN, 400, 400 );
    sleep( 5 );

    return( 0 );
}
