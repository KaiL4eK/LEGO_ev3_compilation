#include <timer.h>

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

	this->DeltaCounts = this->CurrTime - this->PrevTime;

	this->ComputeSecondsPerCount();

	/////////////////////////
	//TotalTime
	/////////////////////////
	if (this->IsInPause)
		this->PauseTime = this->CurrTime - this->StopTime + this->PrevPauseTime;
	
	this->TotalTime = ((this->CurrTime - this->BaseTime) - this->PauseTime) * this->SecondsPerCount;

	/////////////////////////
	//DeltaTime
	/////////////////////////
	this->DeltaTime = (this->DeltaCounts) * this->SecondsPerCount;

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
	this->StopTime = 0;
	this->PauseTime = 0;
	this->PrevPauseTime = 0;
	this->IsInPause = false;
	this->TimeElapsed = 0;
	this->FrameCnt = 0;
}

void timer::Resume()
{
	this->ComputeCurrTime();
	this->PauseTime = this->CurrTime - this->StopTime + this->PrevPauseTime;
	this->PrevPauseTime = this->PauseTime;
	this->IsInPause = false;
}

void timer::Stop()
{
	this->ComputeCurrTime();
	this->StopTime = this->CurrTime;
	this->IsInPause = true;
}

void timer::ComputeSecondsPerCount()
{

	//QueryPerformanceFrequency((LARGE_INTEGER*)&this->CountsPerSecond);
	clock();
	this->SecondsPerCount = 1.f/ CLOCKS_PER_SEC * Scale;

}

float timer::GetPauseTime()
{

	this->ComputeSecondsPerCount();
	return this->PauseTime * this->SecondsPerCount;

}

timer* Timer;

extern "C"
{

	void timer_init()
	{

		Timer = new timer(1.f);

	}

	void timer_start()
	{

		if(Timer)
		{

			Timer->Reset();

		}

	}

	float timer_get_time()
	{

		if(Timer)
		{

			return Timer->GetTotalTime();

		}
		return 0;

	}

	void timer_close()
	{

		if(Timer)
		{

			delete Timer;
			Timer = 0;

		}

	}

	void timer_update()
	{

		if(Timer)
		{

			Timer->Tick();

		}

	}

}