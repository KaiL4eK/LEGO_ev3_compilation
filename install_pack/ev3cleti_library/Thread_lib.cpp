
#include <thread>
#include <mutex>
#include <vector>
#include <functional>

std::vector<std::thread*> Threads;

std::mutex Mutex;

extern "C"
{
	#include <ev3_thread.h>
	uint8_t thread_Init()
	{

		std::thread* NewThread = NULL;

		Threads.push_back(NewThread);

		return Threads.size();

	}
	void thread_Create(void (func)(char*),uint8_t indexOfThread,char* cmd)
	{

		if(indexOfThread < Threads.size())
		{

			Threads.at(indexOfThread) = new std::thread(func,cmd);

		}
		else
			return;

	}

	void thread_Join(uint8_t indexOfThread)
	{

		if(indexOfThread < Threads.size())
		{
			Threads.at(indexOfThread)->join();
		}

	}

	void thread_Detach(uint8_t indexOfThread)
	{
		if(indexOfThread < Threads.size())
		{
			Threads.at(indexOfThread)->detach();
		}
	}
	void thread_ProtectOn()
	{
		Mutex.lock();
	}

	void thread_ProtectOff()
	{
		Mutex.unlock();
	}

}

