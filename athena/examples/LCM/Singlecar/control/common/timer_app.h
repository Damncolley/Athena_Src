#include "local_timer.h"
#include <boost/bind.hpp>
#include <vector>
#include "local_timer.h"
#include <vector>

 using namespace std;

template<class MessageHandlerClass>
class TimerApp
{
    typedef void (MessageHandlerClass::*HandlerMethod)();

    typedef struct{
        int                   interval_;
        HandlerMethod         handler_method_;     //回调函数指针
        MessageHandlerClass*  handler_;    //调用对象
        local_timer *timer;
    }TimerBase;

    public:
        static void AddTimer(int interval,HandlerMethod handler_method,MessageHandlerClass *handler);

    private:
        static std::vector<TimerBase> timer_vec_;
        static void OnTimer(int index);
        static void run();
};

template<class MessageHandlerClass> std::vector<typename TimerApp<MessageHandlerClass>::TimerBase> TimerApp<MessageHandlerClass>::timer_vec_;
//template<class MessageHandlerClass> int TimerApp<MessageHandlerClass>::interval_ = 0;
//template<class MessageHandlerClass> typename TimerApp<MessageHandlerClass>::HandlerMethod TimerApp<MessageHandlerClass>::handler_method_ = NULL;
//template<class MessageHandlerClass> MessageHandlerClass * TimerApp<MessageHandlerClass>::handler_ = NULL;

 template<class MessageHandlerClass>
 void TimerApp<MessageHandlerClass>::OnTimer(int index)
 {
     HandlerMethod handler_method_= timer_vec_[index].handler_method_;
     MessageHandlerClass *handler = timer_vec_[index].handler_;
     (handler->*handler_method_)();
 }

template<class MessageHandlerClass>
void TimerApp<MessageHandlerClass>::AddTimer(int interval,HandlerMethod handler_method,MessageHandlerClass *handler)
{
    TimerBase timerbase;
    timerbase.interval_ = interval;
    timerbase.handler_ = handler;
    timerbase.handler_method_ = handler_method;
    timerbase.timer = new local_timer();
    timer_vec_.push_back(timerbase);
    run();
}

template<class MessageHandlerClass>
void TimerApp<MessageHandlerClass>::run()
{
    int index;
    for(auto it = timer_vec_.begin();it != timer_vec_.end();++it)
    {
        index = it - timer_vec_.begin();
        it -> timer -> StartTimer(it -> interval_,std::bind(OnTimer,std::placeholders::_1),index);
    }
}
