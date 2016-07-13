#include <vector>
#include <sstream>

#include <common.h>
#include <ev3_core.h>

void error_process ( std::string function, std::string msg ) 
{
    Lcd_clear();

    Lcd_text( 5, 10, "Error", NORMAL );
    Lcd_text( 5, 20, function.c_str(), NORMAL );

    unsigned int  text_off = 30,
                  string_off = 0;

    while ( msg.length() > string_off ) {
        std::string out_ = msg.substr( string_off, 21 );
        string_off += 21;
        
        Lcd_text( 5, text_off, out_.c_str(), NORMAL );
        text_off += 10;
    }

    getchar();

    Lcd_release();
    Motor_stor_all();

    exit( 1 );
}
