#include <stdlib.h>
#include <stdio.h>

#include <ev3_sound.h>

#define ARRAY_SIZE( array_ptr ) (sizeof(array_ptr)/sizeof(array_ptr[0]))

int main ( void )
{
    Sound_speak("Hello, I am Robot!", true);
    // Last must have 0.0f delay
    Note_t notes [] = {
            {392, 350, 100}, {392, 350, 100}, {392, 350, 100}, {311.1, 250, 100},
            {466.2, 25, 100}, {392, 350, 100}, {311.1, 250, 100}, {466.2, 25, 100},
            {392, 700, 100}, {587.32, 350, 100}, {587.32, 350, 100},
            {587.32, 350, 100}, {622.26, 250, 100}, {466.2, 25, 100},
            {369.99, 350, 100}, {311.1, 250, 100}, {466.2, 25, 100}, {392, 700, 100},
            {784, 350, 100}, {392, 250, 100}, {392, 25, 100}, {784, 350, 100},
            {739.98, 250, 100}, {698.46, 25, 100}, {659.26, 25, 100},
            {622.26, 25, 100}, {659.26, 50, 400}, {415.3, 25, 200}, {554.36, 350, 100},
            {523.25, 250, 100}, {493.88, 25, 100}, {466.16, 25, 100}, {440, 25, 100},
            {466.16, 50, 400}, {311.13, 25, 200}, {369.99, 350, 100},
            {311.13, 250, 100}, {392, 25, 100}, {466.16, 350, 100}, {392, 250, 100},
            {466.16, 25, 100}, {587.32, 700, 100}, {784, 350, 100}, {392, 250, 100},
            {392, 25, 100}, {784, 350, 100}, {739.98, 250, 100}, {698.46, 25, 100},
            {659.26, 25, 100}, {622.26, 25, 100}, {659.26, 50, 400}, {415.3, 25, 200},
            {554.36, 350, 100}, {523.25, 250, 100}, {493.88, 25, 100},
            {466.16, 25, 100}, {440, 25, 100}, {466.16, 50, 400}, {311.13, 25, 200},
            {392, 350, 100}, {311.13, 250, 100}, {466.16, 25, 100},
            {392.00, 300, 150}, {311.13, 250, 100}, {466.16, 25, 100}, {392, 700, 0}
            };
    Sound_tones(notes, ARRAY_SIZE(notes), true);
	return( 0 );
}
