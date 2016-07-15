#ifndef D3DAPPTIMER_H
#define D3DAPPTIMER_H

#include <stdlib.h>
#include <unistd.h>
#include <time.h>

class D3DAPPTIMER
{
public:

	D3DAPPTIMER(float timeScale);
	~D3DAPPTIMER();

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
	
	float GetSecondsPerCount() { return this->SecondsPerCount; }
	void ComputeSecondsPerCount();
	
	float GetTimeScale() { return this->Scale; }
	
	//Return seconds in one cycle
	float GetDeltaTime() { return this->DeltaTime; }
	
	void ComputeCurrTime() { this->CurrTime = clock(); }
	
	//Return Time (Processor cycles) 
	double& GetCurrTime() { return this->CurrTime; }

	
	bool GetIsStoped() { return this->IsInPause; }



private:

	///////////////////////////////////
	//**Processor Counts
	///////////////////////////////////
	// Current Time
	double CurrTime;
	// Previous Time
	double PrevTime;
	// Start Time
	double BaseTime;
	// Stop Time
	double StopTime;
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

#endif
