#include "ev3dev.h"

using namespace ev3dev;

int main() {
    sound::speak("Hello, I am Robot!", true);
    sound::speak("Hey, Baby, I love you!", true);
    return( 0 );
}