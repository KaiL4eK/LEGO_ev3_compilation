#include <timer.h>

D3DAPPTIMER::D3DAPPTIMER(float timeScale)
{
	this->Scale = timeScale;
}

D3DAPPTIMER::~D3DAPPTIMER()
{

}

void D3DAPPTIMER::Tick()
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

void D3DAPPTIMER::Reset()
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

void D3DAPPTIMER::Resume()
{
	this->ComputeCurrTime();
	this->PauseTime = this->CurrTime - this->StopTime + this->PrevPauseTime;
	this->PrevPauseTime = this->PauseTime;
	this->IsInPause = false;
}

void D3DAPPTIMER::Stop()
{
	this->ComputeCurrTime();
	this->StopTime = this->CurrTime;
	this->IsInPause = true;
}

void D3DAPPTIMER::ComputeSecondsPerCount()
{

	//QueryPerformanceFrequency((LARGE_INTEGER*)&this->CountsPerSecond);
	clock();
	this->SecondsPerCount = 1.f/ CLOCKS_PER_SEC * Scale;

}

float D3DAPPTIMER::GetPauseTime()
{

	this->ComputeSecondsPerCount();
	return this->PauseTime * this->SecondsPerCount;

}

D3DAPPTIMER* Timer;

extern "C"
{

	void TimerInit()
	{

		Timer = new D3DAPPTIMER(1.f);

	}

	void TimerStart()
	{

		if(Timer)
		{

			Timer->Reset();

		}

	}

	float GetTime()
	{

		if(Timer)
		{

			return Timer->GetTotalTime();

		}
		return 0;

	}

	void TimerClose()
	{

		if(Timer)
		{

			delete Timer;
			Timer = 0;

		}

	}

	void TimerUpdate()
	{


		if(Timer)
		{

			Timer->Tick();

		}

	}

}