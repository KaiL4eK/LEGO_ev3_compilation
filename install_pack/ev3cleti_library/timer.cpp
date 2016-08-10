#include <timer.h>
#include <unistd.h>

timer::timer(float timeScale)
{
	this->Scale = timeScale;
}

timer::~timer()
{

}

void timer::Tick()
{

	this->ComputeCurrTime();

	/////////////////////////
	//TotalTime
	/////////////////////////
    if (this->IsInPause)
        this->PauseTime = this->CurrTime.tv_sec - this->StopTime.tv_sec + this->PrevPauseTime;

    this->TotalTime = ((this->CurrTime.tv_sec - this->BaseTime.tv_sec) - this->PauseTime);

	/////////////////////////
	//DeltaTime
	/////////////////////////
    this->DeltaTime = (this->CurrTime.tv_sec - this->PrevTime.tv_sec);

    this->PrevTime = this->CurrTime;

	/////////////////////////
	//fps
	/////////////////////////
	this->FrameCnt++;

	if ((this->TotalTime - this->TimeElapsed) >= 1.0f)
	{
		this->FPS = this->FrameCnt;
		this->TimeElapsed += 1.0f;
		this->FrameCnt = 0;
	}

}

void timer::Reset()
{
    this->ComputeCurrTime();
    this->BaseTime = this->CurrTime;
    this->PrevTime = this->CurrTime;
    this->StopTime.tv_sec = 0;
    this->PauseTime = 0;
    this->PrevPauseTime = 0;
    this->IsInPause = false;
    this->TimeElapsed = 0;
    this->FrameCnt = 0;
}

void timer::Resume()
{
    this->ComputeCurrTime();
    this->PauseTime = this->CurrTime.tv_sec - this->StopTime.tv_sec + this->PrevPauseTime;
    this->PrevPauseTime = this->PauseTime;
    this->IsInPause = false;
}

void timer::Stop()
{
	this->ComputeCurrTime();
	this->StopTime = this->CurrTime;
	this->IsInPause = true;
}

int32_t timer::GetPauseTime()
{

	return this->PauseTime;

}

#include <functional>
#include <iostream>

timer* Timer;
timer* TempTimer;

extern "C"
{

	void Timer_reset()
	{

		if(!Timer)
			Timer = new timer(1.f);	
		Timer->Reset();

	}

	int32_t Timer_get_time()
	{

		if(!Timer)
		{
			Timer = new timer(1.f);
			Timer->Reset();
		}

		Timer->Tick();

		return Timer->GetTotalTime();

	}

	void Timer_close()
	{

		if(Timer)
		{

			delete Timer;
			Timer = 0;

		}

	}

	void Timer_sleep_until(int sec)
	{

		sleep(sec);

	}

	short Timer_count_from(int sec, int* currSec)
	{

		if(!TempTimer)
		{

			TempTimer = new timer(1.f);
			TempTimer->Reset();

		}
		
		TempTimer->Tick();

		if(currSec)
			*currSec = sec - TempTimer->GetTotalTime();

		if(TempTimer->GetTotalTime() >= sec)
		{
			delete TempTimer;
			TempTimer = 0;
			return 1;
		}
		else 
			return 0;

	}

}