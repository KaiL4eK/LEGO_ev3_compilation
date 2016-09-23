#ifndef TIMER_H
#define TIMER_H

void Timer_reset();
float Timer_get_time();
void Timer_close();	
void Timer_sleep_until(float sec);
short Timer_count_from(float sec, float* currSec);

#endif //!TIMER_H