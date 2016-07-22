/*#ifndef THREAD_H
#define THREAD_H

#include <stdlib.h>
#include <pthread.h>
#include <vector>

class Thread
{

public:

    Thread(){}
    ~Thread(){}

    void SetArg(void* arg) { this->Args.push_back(arg); }
    std::vector<void*> GetArg() { return this->Args; }

    pthread_t* GetThread() {return this->Thr;}
    void SetThread(pthread_t* thr) {this->Thr = thr;}

    void Detach();
    void Join();

private:

    pthread_t* Thr;
    std::vector<void*> Args;

};

void InitThread(Thread* thr, void*(*func)(void*));

#endif //!THREAD_H*/