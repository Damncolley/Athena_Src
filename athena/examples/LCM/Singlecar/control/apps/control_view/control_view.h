/**
 * @file    control_view.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef APPS_CONTROL_VIEW_CONTROL_VIEW_H_
#define APPS_CONTROL_VIEW_CONTROL_VIEW_H_

#include <GL/glu.h>
#include <GL/glut.h>

//#include "controller.h"
#include "../../control_logic/control_logic.h"


 /**
 * @namespace athena::control
 * @brief athena::control
 */
namespace athena{
namespace control{

 /**
 * @class ControlView
 * @brief 调试窗口.
 */
class ControlView{
 public:
  #define KEY_ESC  27
  ControlView()  = default;
  ~ControlView() = default;

  static void DrawPath();

  static void DrawText(float x, float y, float z, char * outputstring);
  static void DrawTextRGB(float x, float y, float z, float r, float g, float b, char * outputstring);

  static void OpenGL_Draw();

  static void MyGLDispIni();
  static void myDisplay();

  static void SpecialKey(int key, int x, int y);
  static void MouseKey(int button, int state, int x, int y);
  static void MouseRotate(int x, int y, int z);
  static void PassiveMouseMove(int x, int y);
  static void MouseMove(int x, int y);
  static void Reshape(int w, int h);
  static void Draw_Best_March_Point(path& p,  map_matching& matching);
  static void Key(unsigned char key, int x, int y);

  static void Init(ControlLogic*control_logic,double vehicle_width);


  static void Draw_Org();
  static void Draw_Point(double x, double y);
  static void DrawCar(double x, double y,  double yaw, double steer_angle );
  static void DrawCar_e(double x, double y,  double x_c, double  y_c, double yaw, double yaw_c);

  static void Draw_Virtual_lane(path& v_p,int output_st_pos,int output_en_pos,int every,int r, int g, int b);
  static void Draw_Best_March_Point(path& p,  int no_points);

 private:
    static int x_lbefore;
    static int y_lbefore;
    static int x_rbefore;
    static int y_rbefore;
    static int z_before1;
    static int z_before2;

    static bool buttonSaveLeft;
    static bool buttonSaveMiddle;
    static bool buttonSaveRight;
    static float x_move;
    static float y_move;
    static float z_move;
    static float x_move_save;
    static float y_move_save;
    static float z_move_save;
    static float x_rotate;
    static float y_rotate;
    static float z_rotate;
    static float x_rotate_save;
    static float y_rotate_save;
    static float z_rotate_save;
    static float m_zoom;

    static float m_aspect;

    static float m_eyex;
    static float m_eyey;
    static float m_eyez;
    static float m_eyeyaw;
    static float m_centerx;
    static float m_centery;
    static float m_centerz;
    static float m_upx;
    static float m_upy;
    static float m_upz;
    static int g_frame;
    static bool g_pause;
    static double front_track;
    static double front_wheel_wide;
    static double back_wheel_wide;

    static double vehicle_width_;

    static int32_t match_point_num_;
    static double position_x_c;
    static double position_y_c;
    static double yaw_c;

    static ControlLogic *control_logic_;
    static DebugOutput debug_output_;

    static double tar_speed_debug_;
};
}
}

#endif //APPS_CONTROL_VIEW_CONTROL_VIEW_H_
