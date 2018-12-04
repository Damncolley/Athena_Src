#include "Thread.h"
 
Thread::Thread(){
   
}

Thread::~Thread(){   
}

 
void* Thread::run0(void* opt)
{
     Thread* p = (Thread*) opt;
     p->run1();
     return p;
}

void* Thread::run1()
{
    _tid = pthread_self();
    run();
     _tid = 0;
     pthread_exit(NULL);
}
 
bool Thread::start()
{
     return pthread_create(&_tid, NULL, run0, this) == 0;
}

void Thread::join()
{
     if( _tid > 0 ){
         pthread_join(_tid, NULL);
     }
 }
