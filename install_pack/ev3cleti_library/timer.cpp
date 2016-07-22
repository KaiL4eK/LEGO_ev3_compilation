#include <timer.h>
#include <unistd.h>
//#include <Thread.h>

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

//#include <thread>
#include <functional>
#include <iostream>

timer* Timer;
timer* TempTimer;

//uint8_t& command, float& time
void *ThreadTimerGetTime(void* args)
{
	//StrArgs* Args = (StrArgs*)args; 
	printf("Hello, before new Timer\n");
    timer* CTimer = new timer(0.5f);

    uint8_t* Cmd = (uint8_t*)args;
    int32_t* Time = (int32_t*)((uint8_t*)args + 1);

    CTimer->Reset();
    do
    {
        if(*Cmd == 1)
            break;
        CTimer->Tick();
        *Time = CTimer->GetTotalTime();
        printf("In Thread time = %d \n", *Time);

    }while(true);

	printf("Hello, after Timer Exe\n");
    *Cmd = 0;
    delete CTimer;
}

extern "C"
{

	/*void ThreadTimeDelay(int seconds, short& ready)
	{

	    timer* CTimer = new timer(0.5f);

	    CTimer->Reset();
	    do
	    {
	        CTimer->Tick();

	    }while(CTimer->GetTotalTime() < seconds);

	    delete CTimer;

	    ready = 1;

	}*/

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

	}*/

	uint8_t Command = 0;

	void TimerStop()
	{

		Command = 1;

	}

	/*float TimerGetTime()
	{

		static bool Status;
		static int32_t Time;
		static Thread Thr;

	    if(!Status)
	    {
	    	Thr.SetArg(&Command);
	    	Thr.SetArg(&Time);
	    	InitThread(&Thr, ThreadTimerGetTime);
	        Thr.Detach();
	        Status = true;
	    }
	    return Time;

	    /*static bool Status = 0;
	    static float Time;

	    if(!Timer)
		{
			Timer = new timer(1.f);
			Timer->Reset();
		}

		return Timer->ThGetTime(Command, Time, Status);

	}*/

}