#ifndef TIMER_H
#define TIMER_H

#include <device.h>
#include <sys/time.h>

class timer
{
public:

    timer(float timeScale);
    ~timer();

    // Reset Timer
    void Reset();

    // Update Timer
    // Must be executed every cycle
    void Tick();

    void Stop();
    void Resume();

    // Change Timer Speed
    // TimeScale = 1.f - default
    void SetTimeScale(float timeScale) { this->Scale = timeScale; }

    // Return Fraps(cycles) per second
    int GetFPS(){ return FPS; }
    // Return Pause Time
    int32_t GetPauseTime();
    // Return non-Pause Time
    float GetTotalTime() { return this->TotalTime; }

    void ComputeSecondsPerCount();

    float GetTimeScale() { return this->Scale; }

    //Return seconds in one cycle
    float GetDeltaTime() { return this->DeltaTime; }

    void ComputeCurrTime() { gettimeofday(&this->CurrTime,NULL); }

    //Return Time (Processor cycles)
    float GetCurrTime() { return this->CurrTime.tv_sec + this->CurrTime.tv_usec/1000000000; }

    bool GetIsStoped() { return this->IsInPause; }

    float ThGetTime(uint8_t& command, float& time, bool& status);


private:

    ///////////////////////////////////
    //**Processor Counts
    ///////////////////////////////////
    // Current Time
    timeval CurrTime;
    // Previous Time
    timeval PrevTime;
    // Start Time
    timeval BaseTime;
    // Stop Time
    timeval StopTime;
    // In Pause Time
    float PauseTime;
    // In Previuos Pause Time
    float PrevPauseTime;

    ///////////////////////////////////
    //**Non-Processor Counts
    ///////////////////////////////////
    int FPS;

    float Scale;
    // non-Pause Time
    float TotalTime;

    float DeltaTime;

private:

    int FrameCnt;
    float TimeElapsed;

    bool IsInPause;
};
#endif //!TIMER_H
