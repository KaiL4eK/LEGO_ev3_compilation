

#include <thread>
#include <mutex>
#include <vector>
#include <functional>

std::vector<std::thread*> Threads;

std::mutex Mutex;

extern "C"
{
	#include <ev3_thread.h>
	uint8_t InitThread()
	{

		std::thread* NewThread = NULL;

		Threads.push_back(NewThread);

		return Threads.size();

	}
	void CreateThread(void (func)(char*),uint8_t indexOfThread,char* cmd)
	{

		if(indexOfThread < Threads.size())
		{

			Threads.at(indexOfThread) = new std::thread(func,cmd);

		}
		else
			return;

	}

	void JoinThread(uint8_t indexOfThread)
	{

		if(indexOfThread < Threads.size())
		{
			Threads.at(indexOfThread)->join();
		}

	}

	void ProtectOn()
	{
		Mutex.lock();
	}
	void ProtectOff()
	{
		Mutex.unlock();
	}

}

