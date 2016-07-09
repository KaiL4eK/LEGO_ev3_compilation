#include <string>
#include <sstream>
#include <iostream>
#include <vector>

extern "C" {

#include <ev3_sound.h>

int Sound_beep(const char *args, bool bSynchronous)
{
    std::ostringstream cmd;
    cmd << "/usr/bin/beep " << std::string(args);
    if (!bSynchronous) 
        cmd << " &";
    return( std::system(cmd.str().c_str()) );
}

//-----------------------------------------------------------------------------

void Sound_tones(Note_t notes[], size_t array_size, bool bSynchronous)
{
    std::ostringstream args;
    bool first = true;

    for ( size_t i = 0; i < array_size; i++ ) {

        if ( first ) {
            first = false;
        } else {
            args << " -n";
        }

        args << " -f " << notes[i].frequency << " -l " << notes[i].length << " -D " << notes[i].delay;

        if ( notes[i].delay == 0.0f ) {
            break;
        }
    }
    Sound_beep(args.str().c_str(), bSynchronous);
}

//-----------------------------------------------------------------------------

void Sound_tone(float frequency, float ms, bool bSynchronous) {
    Note_t notes [] = {{frequency, ms, 0.0f}};
    Sound_tones(notes, 1, bSynchronous);
}

//-----------------------------------------------------------------------------

int Sound_play(const char *soundfile, bool bSynchronous)
{
    std::ostringstream cmd;
    cmd << "/usr/bin/aplay -q " << std::string(soundfile);

    if (!bSynchronous) 
        cmd << " &";

    return( std::system(cmd.str().c_str()) );
}

//-----------------------------------------------------------------------------

int Sound_speak (const char *speak_string, bool bSynchronous)
{
    std::ostringstream cmd;

    cmd << "/usr/bin/espeak -a 200 --stdout \"" << std::string(speak_string) << "\""
     << " | /usr/bin/aplay -q";

    if (!bSynchronous) 
        cmd << " &";

    return( std::system(cmd.str().c_str()) );
}

}
