#ifndef TIMER_H
#define TIMER_H

#include <ev3_core.h>

void TimerInit();
void TimerStart();
float GetTime();
void TimerClose();
void TimerUpdate();

#endif //!TIMER_H