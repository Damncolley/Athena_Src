#include "timer_app.h"

namespace athena{
namespace control{
 void TimerApp::add_timer(func OnTimer)
 {
    timer.StartTimer(10, std::bind(OnTimer));
 }
}
}
