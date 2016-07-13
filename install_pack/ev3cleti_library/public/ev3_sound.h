#ifndef EV3_SOUND_H_
#define EV3_SOUND_H_

typedef struct note_ {
    float   frequency,
            length,
            delay;
}Note_t;

int Sound_speak (const char *speak_string, bool bSynchronous);
void Sound_tone(float frequency, float ms, bool bSynchronous);
void Sound_tones(Note_t notes[], size_t array_size, bool bSynchronous);
int Sound_play(const char *soundfile, bool bSynchronous);
int Sound_beep(const char *args, bool bSynchronous);

#endif //EV3_SOUND_H_
