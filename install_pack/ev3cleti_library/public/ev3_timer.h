#ifndef TIMER_H
#define TIMER_H

void Timer_reset();
int32_t Timer_get_time();
void Timer_close();	
void Timer_sleep_until(int sec);
short Timer_count_from(int sec, int* currSec);
//short TimeDelay(int seconds);
//float TimerGetTime();
//void TimerStop();

#endif //!TIMER_H