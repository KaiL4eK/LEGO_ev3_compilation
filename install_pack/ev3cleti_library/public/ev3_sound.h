#ifndef SOUND_H_
#define SOUND_H_

#ifndef __cplusplus
typedef enum bool_ {
	false = 0,
	true = 1
}bool;
#endif

typedef struct note_ {
    float   frequency,
            length,
            delay;
}Note_t;

void Sound_speak (const char *speak_string, bool bSynchronous);
void Sound_tone(float frequency, float ms, bool bSynchronous);
void Sound_tones(Note_t notes[], bool bSynchronous);
void Sound_play(const char *soundfile, bool bSynchronous);
void Sound_beep(const char *args, bool bSynchronous);

#endif //SOUND_H_
