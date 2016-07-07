#include <string>
#include <sstream>
#include <iostream>

class sound
{
public:
  // static void beep(const std::string &args = "", bool bSynchronous = false);
  // static void tone(float frequency, float ms, bool bSynchronous = false);
  // static void tone(const std::vector< std::vector<float> > &sequence, bool bSynchronous = false);
  // static void play(const std::string &soundfile, bool bSynchronous = false);
  static void speak(const std::string &text, bool bSynchronous = false);
};

extern "C" void Sound_speak (const char *speak_string)
{
	std::cout << "Speak =)" << std::endl;
    // sound::speak(std::string(speak_string), true);
}

// void sound::speak(const std::string &text, bool bSynchronous)
// {
//   std::ostringstream cmd;

//   cmd << "/usr/bin/espeak -a 200 --stdout \"" << text << "\""
//       << " | /usr/bin/aplay -q";

//   if (!bSynchronous) cmd << " &";

//   std::system(cmd.str().c_str());
// } 
