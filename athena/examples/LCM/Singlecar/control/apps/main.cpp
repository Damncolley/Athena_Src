#include <GL/glu.h>
#include <GL/glut.h>
#include <glog/logging.h>
#include "control.h"
//#include "control_debug/control_debug.h"

int main(int argc, char **argv) {
    std::string ver = "control start--20181026V1";
    glutInit(&argc, argv);
    system("mkdir -p log");
    google::InitGoogleLogging(argv[0]);

    Control *control = new Control(ver);
    control -> Init();
    control -> start();
   // ControlDebug::Init(&control);
    glutMainLoop();
    glutMainLoop();
    control -> join();
    return 0;
}
