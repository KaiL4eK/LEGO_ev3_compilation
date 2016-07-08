#ifndef SOUND_H_
#define SOUND_H_

typedef enum bool_ {
	true = 1,
	false = 0
}bool;

void Sound_speak (const char *speak_string, bool bSynchronous);
void Sound_beep(const std::string &args, bool bSynchronous);
void Sound_tone(float frequency, float ms, bool bSynchronous);
void Sound_tone_vect(const std::vector< std::vector<float> > &sequence, bool bSynchronous);
void Sound_play(const std::string &soundfile, bool bSynchronous);

#endif //SOUND_H_
