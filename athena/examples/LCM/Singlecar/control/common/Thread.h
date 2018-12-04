#ifndef COMMUNITCATE_H
#define COMMUNITCATE_H



#include <pthread.h>

class Thread
{
protected:
    pthread_t _tid;
     static void* run0(void* opt);
    void* run1();//如果类中有保存线程状态的变量，可以在这个函数中可以进行更改操作
public:
     Thread();
     ~Thread();
     /**
      * 创建线程，线程函数是 run0
      *
     * @return 成功返回 ture 否则返回 false
      */
     bool start();
     /**
      * join this thread
      *
      */
     void join();
     virtual void run(){

     }
 };


 #endif



