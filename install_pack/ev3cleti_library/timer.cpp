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

	
	this->ComputeSecondsPerCount();
	this->ComputeCurrTime();

	/////////////////////////
	//TotalTime
	/////////////////////////
	if (this->IsInPause)
		this->PauseTime = this->CurrTime - this->StopTime + this->PrevPauseTime;
	
	this->TotalTime = ((this->CurrTime - this->BaseTime) - this->PauseTime) * this->SecondsPerCount;

	/////////////////////////
	//DeltaTime
	/////////////////////////
	this->DeltaTime = (this->CurrTime - this->BaseTime) * this->SecondsPerCount;

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

void ThreadTimeDelay(int seconds, short& ready)
{

    timer* CTimer = new timer(0.5f);

    CTimer->Reset();
    do
    {
        CTimer->Tick();

    }while(CTimer->GetTotalTime() < seconds);

    delete CTimer;

    ready = 1;

}

void ThreadTimerGetTime(uint8_t& command, float& time)
{

    timer* CTimer = new timer(0.5f);

    CTimer->Reset();
    do
    {
        if(command == 1)
            break;
        CTimer->Tick();
        time = CTimer->GetTotalTime();

    }while(true);

    command = 0;
    delete CTimer;
}

timer* Timer;

uint8_t Command = 0;

extern "C"
{

	void Timer_reset()
	{

		if(!Timer)
			Timer = new timer(1.f);	
		Timer->Reset();

	}

	float Timer_get_time()
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

	///////////////////////////////////////
	//**Timer funcs for parallel execution
	//**This functions just for tests
	//**I will rename them later
	///////////////////////////////////////



	/*short TimeDelay(int seconds)
	{

	    static short Ready = 0;
	    static bool Status = 0;

	    if(!Status)
	    {
	        std::thread ThTimeDelay(ThreadTimeDelay,seconds, std::ref(Ready));
	        //ThTimeDelay.detach();
	        Status = true;
	    }

	    return Ready;

	}

	float TimerGetTime()
	{

	    static bool Status;
	    static float Time;

	    if(!Status)
	    {
	        //std::thread Thread(ThreadTimerGetTime, std::ref(Command), std::ref(Time));
	        //Thread.detach();
	        Status = true;
	    }

	    return Time;

	}*/

}