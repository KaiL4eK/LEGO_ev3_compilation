#include <string>
#include <sstream>
#include <iostream>
#include <vector>

extern "C" {

#include <ev3_sound.h>

void Sound_beep(const char *args, bool bSynchronous)
{
  std::ostringstream cmd;
  cmd << "/usr/bin/beep " << std::string(args);
  if (!bSynchronous) cmd << " &";
  std::system(cmd.str().c_str());
}

//-----------------------------------------------------------------------------

void Sound_tones(Note_t notes[], bool bSynchronous)
{
    std::ostringstream args;
    bool first = true;

    for ( int i = 0; ; i++ ) {

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
    Sound_tones(notes, bSynchronous);
}

//-----------------------------------------------------------------------------

void Sound_play(const char *soundfile, bool bSynchronous)
{
  std::ostringstream cmd;
  cmd << "/usr/bin/aplay -q " << std::string(soundfile);

  if (!bSynchronous) cmd << " &";

  std::system(cmd.str().c_str());
}

//-----------------------------------------------------------------------------

void Sound_speak (const char *speak_string, bool bSynchronous)
{
  std::ostringstream cmd;

  cmd << "/usr/bin/espeak -a 200 --stdout \"" << std::string(speak_string) << "\""
      << " | /usr/bin/aplay -q";

  if (!bSynchronous) cmd << " &";

  std::system(cmd.str().c_str());
}

}
