#ifndef TIMER_H
#define TIMER_H

#include <stdlib.h>
#include <unistd.h>
#include <time.h>

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
	float GetPauseTime();
	// Return non-Pause Time
	float GetTotalTime() { return this->TotalTime; }
	
	clock_t GetSecondsPerCount() { return this->SecondsPerCount; }
	void ComputeSecondsPerCount();
	
	float GetTimeScale() { return this->Scale; }
	
	//Return seconds in one cycle
	clock_t GetDeltaTime() { return this->DeltaTime; }
	
	void ComputeCurrTime() { this->CurrTime = clock(); }
	
	//Return Time (Processor cycles) 
	clock_t& GetCurrTime() { return this->CurrTime; }

	
	bool GetIsStoped() { return this->IsInPause; }



private:

	///////////////////////////////////
	//**Processor Counts
	///////////////////////////////////
	// Current Time
	clock_t CurrTime;
	// Previous Time
	clock_t PrevTime;
	// Start Time
	clock_t BaseTime;
	// Stop Time
	clock_t StopTime;
	// In Pause Time
	double PauseTime;
	// In Previuos Pause Time
	double PrevPauseTime;

	double CountsPerSecond;
	double SecondsPerCount;

	///////////////////////////////////
	//**Non-Processor Counts
	///////////////////////////////////
	int FPS;
	
	float Scale;
	// non-Pause Time
	float TotalTime;
	
	float DeltaTime;
	float DeltaCounts;

private:

	int FrameCnt;
	float TimeElapsed;

	bool IsInPause;
};

#endif //!TIMER_H
