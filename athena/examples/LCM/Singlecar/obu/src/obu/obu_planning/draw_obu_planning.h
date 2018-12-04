#pragma once

#define KEY_ESC  27

extern int x_lbefore,y_lbefore;
extern int x_rbefore,y_rbefore;
extern int z_before1,z_before2;

extern bool buttonSaveLeft,  buttonSaveMiddle,  buttonSaveRight;
extern float x_move,y_move,z_move;
extern float x_move_save,y_move_save, z_move_save;
extern float x_rotate,y_rotate,z_rotate;
extern float x_rotate_save,y_rotate_save,z_rotate_save;
extern float m_zoom;

extern float m_aspect;

extern float m_eyex, m_eyey,  m_eyez;
extern float m_centerx,  m_centery, m_centerz;
extern float m_upx, m_upy, m_upz;

///////////////////OPEN GL control ///////////////////////////////////////
extern int g_frame;
extern bool g_pause;

void DrawPath();
void draw_trajectorys_motion_plan();

void DrawText(float x, float y, float z, char * outputstring);
void DrawTextRGB(float x, float y, float z, float r, float g, float b, char * outputstring);

///////////////////////////////////////////////////////////////
void OpenGL_Draw();
void DrawCar(double x, double y,  double yaw, double steer_angle);

void MyGLDispIni();
void myDisplay();

void SpecialKey(int key, int x, int y);
void MouseKey(int button, int state, int x, int y);
void MouseRotate(int x, int y, int z);
void PassiveMouseMove(int x, int y);
void MouseMove(int x, int y);
void Reshape(int w, int h);
