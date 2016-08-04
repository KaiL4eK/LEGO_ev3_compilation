#include <Thread.h>

void Thread::Detach()
{

    pthread_detach(*this->GetThread());

}

void Thread::Join()
{

    pthread_join(*this->Thr, NULL);

}

void InitThread(Thread* thr, void*(*func)(void*))
{

    pthread_t PThread;

    pthread_create(&PThread,NULL, func, thr->GetArg().front());
    thr->SetThread(&PThread);

}