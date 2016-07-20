#ifndef TIMER_H
#define TIMER_H

#include <device.h>
#include <pthread.h>

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
	
	int64_t GetSecondsPerCount() { return this->SecondsPerCount; }
	void ComputeSecondsPerCount();
	
	float GetTimeScale() { return this->Scale; }
	
	//Return seconds in one cycle
	int64_t GetDeltaTime() { return this->DeltaTime; }
	
	void ComputeCurrTime() { this->CurrTime = clock(); }
	
	//Return Time (Processor cycles) 
	int64_t& GetCurrTime() { return this->CurrTime; }

	
	bool GetIsStoped() { return this->IsInPause; }

	float ThGetTime(uint8_t& command, float& time, bool& status);


private:

	///////////////////////////////////
	//**Processor Counts
	///////////////////////////////////
	// Current Time
	int64_t CurrTime;
	// Previous Time
	int64_t PrevTime;
	// Start Time
	int64_t BaseTime;
	// Stop Time
	int64_t StopTime;
	// In Pause Time
	int64_t PauseTime;
	// In Previuos Pause Time
	int64_t PrevPauseTime;

	double SecondsPerCount;

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
