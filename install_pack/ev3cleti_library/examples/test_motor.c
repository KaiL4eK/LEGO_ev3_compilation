#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

#include <ev3_motor.h>

int main ( void )
{
    while ( true ) {

        if ( Motor_isConnected( OUTPUT_A ) ) {
            Motor_run_forever( OUTPUT_A, 100 );
            sleep( 1 );
            Motor_stop( OUTPUT_A );
            sleep( 1 );
        }
        else {
            printf("Motor is not connected =(\n");
            break;
        }
    }
    return( 0 );
}