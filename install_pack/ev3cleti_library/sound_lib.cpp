#include <string>
#include <sstream>
#include <iostream>
#include <vector>

extern "C" {

void Sound_beep(const std::string &args, bool bSynchronous)
{
  std::ostringstream cmd;
  cmd << "/usr/bin/beep " << args;
  if (!bSynchronous) cmd << " &";
  std::system(cmd.str().c_str());
}

//-----------------------------------------------------------------------------

void Sound_tone_vect(
    const std::vector< std::vector<float> > &sequence,
    bool bSynchronous
    )
{
  std::ostringstream args;
  bool first = true;

  for(auto v : sequence) {
    if (first) {
      first = false;
    } else {
      args << " -n";
    }

    if (v.size() > 0) {
      args << " -f " << v[0];
    } else {
      continue;
    }

    if (v.size() > 1) {
      args << " -l " << v[1];
    } else {
      continue;
    }

    if (v.size() > 2) {
      args << " -D " << v[2];
    } else {
      continue;
    }
  }

  Sound_beep(args.str(), bSynchronous);
}

//-----------------------------------------------------------------------------

void Sound_tone(float frequency, float ms, bool bSynchronous) {
  Sound_tone_vect({{frequency, ms, 0.0f}}, bSynchronous);
}

//-----------------------------------------------------------------------------

void Sound_play(const std::string &soundfile, bool bSynchronous)
{
  std::ostringstream cmd;
  cmd << "/usr/bin/aplay -q " << soundfile;

  if (!bSynchronous) cmd << " &";

  std::system(cmd.str().c_str());
}

void Sound_speak (const char *speak_string, bool bSynchronous)
{
  std::ostringstream cmd;

  cmd << "/usr/bin/espeak -a 200 --stdout \"" << std::string(speak_string) << "\""
      << " | /usr/bin/aplay -q";

  if (!bSynchronous) cmd << " &";

  std::system(cmd.str().c_str());
}

}
