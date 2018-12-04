#include "control_view.h"

namespace athena{
namespace control{

int ControlView::x_lbefore = 0.0;
int ControlView::y_lbefore = 0.0;
int ControlView::x_rbefore = 0.0;
int ControlView::y_rbefore = 0.0;
int ControlView::z_before1 = 0.0;
int ControlView::z_before2 = 0.0;

bool ControlView::buttonSaveLeft = false;
bool ControlView::buttonSaveMiddle = false;
bool ControlView::buttonSaveRight = false;
float ControlView::x_move = 0.0;
float ControlView::y_move = 0.0;
float ControlView::z_move = 0.0;
float ControlView::x_move_save = 0.0;
float ControlView::y_move_save = 0.0;
float ControlView::z_move_save = 0.0;
float ControlView::x_rotate = 0.0;
float ControlView::y_rotate = 0.0;
float ControlView::z_rotate = 0.0;
float ControlView::x_rotate_save = 0.0;
float ControlView::y_rotate_save = 0.0;
float ControlView::z_rotate_save = 0.0;
float ControlView::m_zoom = 0.0;

float ControlView::m_aspect = 0.0;

float ControlView::m_eyex = 0.0;
float ControlView::m_eyey = 0.0;
float ControlView::m_eyez = 0.0;
float ControlView::m_eyeyaw = 0.0;
float ControlView::m_centerx = 0.0;
float ControlView::m_centery = 0.0;
float ControlView::m_centerz = 0.0;
float ControlView::m_upx = 0.0;
float ControlView::m_upy = 0.0;
float ControlView::m_upz = 0.0;
int ControlView::g_frame = 0;
bool ControlView::g_pause;
double ControlView::front_track = 1.2;
double ControlView::front_wheel_wide = 1;
double ControlView::back_wheel_wide = 1;

double ControlView::vehicle_width_ = 1.7;

int32_t ControlView::match_point_num_ = 0;
double ControlView::position_x_c = 0.0;
double ControlView::position_y_c = 0.0;
double ControlView::yaw_c = 0.0;

ControlLogic *ControlView::control_logic_;
DebugOutput ControlView::debug_output_;

double ControlView::tar_speed_debug_ = 0.0;

void ControlView::OpenGL_Draw()
{
    x_move = 0,y_move = 0,z_move = 0;
    x_rotate =1,y_rotate=1,z_rotate=1;
    m_zoom=1;
    g_frame=0;
}

void ControlView::Reshape(int w, int h)
{
    glViewport(0, 0, (GLint)w, (GLint)h);

    m_aspect = (GLfloat) w / (GLfloat) h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f,
                   m_aspect,
                   0.0f,
                   4000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void ControlView::MouseMove(int x, int y)
{
    int mod = glutGetModifiers();
    switch(mod)
    {
    case GLUT_ACTIVE_CTRL :
        x_rotate += (y - z_move_save)/100;
        if (x_rotate > 360)
            x_rotate=x_rotate - 360;
        if (x_rotate < -360)
            x_rotate=x_rotate + 360;
        return;

    case GLUT_ACTIVE_SHIFT :
        y_rotate += (y - z_move_save)/100;
        if (y_rotate > 360)
            y_rotate=y_rotate - 360;
        if (y_rotate < -360)
            y_rotate=y_rotate + 360;
        return;

    case GLUT_ACTIVE_ALT :
        float temp = (x - x_move_save)/100;
        z_rotate += atanf(temp);
        return;

    }

    if(buttonSaveLeft)
    {
        x_move += (x - x_move_save)/100;
        z_move += (y - z_move_save)/100;
    }

    if(buttonSaveMiddle)
    {
        float multiplay = (y - z_move_save)/1000000;
        m_zoom =m_zoom*(1+multiplay);
    }

    if(buttonSaveRight)
    {
        float multiplay = (y - z_move_save)/1000000;
        m_zoom =m_zoom*(1+multiplay);
    }

}

void ControlView::PassiveMouseMove(int x, int y)
{

}

void ControlView::MouseRotate(int x, int y, int z)
{
    //    cout << "mouse Rotate "  << x  << "  "<< y << "  "<< z <<endl;
}

void ControlView::MouseKey(int button, int state, int x, int y)
{
    x_move_save=x;
//    y_move_save;
    z_move_save=y;

    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        if(state == GLUT_DOWN)
            buttonSaveLeft=true;
        else
            buttonSaveLeft=false;
        break;

    case GLUT_MIDDLE_BUTTON:
        if(state == GLUT_DOWN)
            buttonSaveMiddle=true;
        else
            buttonSaveMiddle=false;
        break;

    case GLUT_RIGHT_BUTTON:
        if(state == GLUT_DOWN)
            buttonSaveRight=true;
        else
            buttonSaveRight=false;
        break;
    }
}

void ControlView::Key(unsigned char key, int x, int y)
{
    switch (key)
    {
    case KEY_ESC:
        control_logic_ -> SetDrivingModeDebug(1);
        control_logic_ -> SetTarSpeedDebug(0,false);
        break;
    }
}

void ControlView::SpecialKey(int key, int x, int y)
{
    int mod = 0;
    switch (key)
    {
    case GLUT_KEY_UP:
        mod = glutGetModifiers();
        if (mod == GLUT_ACTIVE_ALT)
        {

        }
        else if (mod == GLUT_ACTIVE_SHIFT)
        {

        }
        else if (mod == GLUT_ACTIVE_CTRL)
        {

        }
        else
            y_move ++;

        break;

    case GLUT_KEY_DOWN:
        mod = glutGetModifiers();
        if (mod == GLUT_ACTIVE_ALT)
        {

        }
        else if (mod == GLUT_ACTIVE_SHIFT)
        {

        }
        else if (mod == GLUT_ACTIVE_CTRL)
        {

        }
        else
            y_move --;
        break;

    case GLUT_KEY_LEFT:
        mod = glutGetModifiers();
        if (mod == GLUT_ACTIVE_ALT)
        {

        }
        else if (mod == GLUT_ACTIVE_SHIFT)
        {

        }
        else if (mod == GLUT_ACTIVE_CTRL)
        {

        }
        else
            x_move --;
        break;

    case GLUT_KEY_RIGHT:
        mod = glutGetModifiers();
        if (mod == GLUT_ACTIVE_ALT)
        {

        }
        else if (mod == GLUT_ACTIVE_SHIFT)
        {

        }
        else if (mod == GLUT_ACTIVE_CTRL)
        {

        }
        else
            x_move ++;
        break;

    case GLUT_KEY_PAGE_UP:
        m_zoom= 1.1*m_zoom;
        break;

    case GLUT_KEY_PAGE_DOWN	:
        m_zoom =m_zoom/1.1;
        break;

    case GLUT_KEY_HOME:
        m_zoom=1.5*m_zoom;
        break;

    case GLUT_KEY_END:
        m_zoom=m_zoom/1.5;
        break;

    case  GLUT_KEY_F1:
        control_logic_ -> SetDrivingModeDebug(3);
        break;

    case  GLUT_KEY_F2:
        x_rotate += -3;
        if (x_rotate > 360)
            x_rotate=x_rotate - 360;
        if (x_rotate < -360)
            x_rotate=x_rotate + 360;
        break;

    case  GLUT_KEY_F3:
        y_rotate += 3;
        if (y_rotate > 360)
            y_rotate=y_rotate - 360;
        if (y_rotate < -360)
            y_rotate=y_rotate + 360;
        break;

    case  GLUT_KEY_F4:
        y_rotate += -3;
        if (y_rotate > 360)
            y_rotate=y_rotate - 360;
        if (y_rotate < -360)
            y_rotate=y_rotate + 360;
        break;

    case  GLUT_KEY_F5:

        z_rotate += atanf(3);
        break;

    case GLUT_KEY_F6:

        z_rotate += atanf(-3);
        break;

    case GLUT_KEY_F9:


        break;

    case GLUT_KEY_F10:
        control_logic_ -> SetDrivingModeDebug(0);
        break;

    case GLUT_KEY_F11:
        tar_speed_debug_ += 2;
        control_logic_ -> SetTarSpeedDebug(tar_speed_debug_,true);
        break;

    case GLUT_KEY_F12:
        tar_speed_debug_ -= 2;
        if(tar_speed_debug_ < 0)
            tar_speed_debug_ = 0;
        control_logic_ -> SetTarSpeedDebug(tar_speed_debug_,true);
       // g_A60_controller->b_motion_plann_continue = true;
      //  cout << " b_motion_plann_continue " << g_A60_controller->b_motion_plann_continue << endl;
        break;

    }
    glutPostRedisplay();
}

void ControlView::myDisplay(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glLoadIdentity();
/*
    m_eyex = g_A60_controller->CurrentX;
    m_eyey = g_A60_controller->CurrentY;
    m_eyez = g_A60_controller->CurrentZ + 20;

    m_centerx = g_A60_controller->CurrentX  + 40 * sin( to_radians(g_A60_controller->Current_heading));
    m_centery = g_A60_controller->CurrentY  + 40 * cos( to_radians(g_A60_controller->Current_heading));
    m_centerz = g_A60_controller->CurrentZ;

    m_upx = g_A60_controller->CurrentX;
    m_upy = g_A60_controller->CurrentY ;
    m_upz =  g_A60_controller->CurrentZ ;
*/
    debug_output_.path_.ref_points_.clear();
    control_logic_ -> GetControllerInfo(debug_output_);
   // std::cout<<"match_point_num_:"<<debug_output_.match_point_num_<<endl;
    m_eyex = debug_output_.local_localization_.current_x_;
    m_eyey = debug_output_.local_localization_.current_y_;
    m_eyez = debug_output_.local_localization_.current_z_ + 30;
    m_eyeyaw = debug_output_.local_localization_.current_yaw_;
    if(debug_output_.path_.IsEmpty() == false)
    {
        match_point_num_ = debug_output_.match_point_num_;
        position_x_c = debug_output_.path_.ref_points_[match_point_num_].position_x_;
        position_y_c = debug_output_.path_.ref_points_[match_point_num_].position_y_;
        yaw_c        = debug_output_.path_.ref_points_[match_point_num_].heading_;
    }
    else{
        position_x_c = 0;
        position_y_c = 0;
        yaw_c        = 0;
    }
    m_centerx = debug_output_.local_localization_.current_x_;
    m_centery = debug_output_.local_localization_.current_y_;
    m_centerz = debug_output_.local_localization_.current_z_;

    m_upx = debug_output_.local_localization_.current_x_;
    m_upy = debug_output_.local_localization_.current_y_ + 10;
    m_upz =  debug_output_.local_localization_.current_z_;
    gluLookAt(m_eyex, 	  m_eyey, 	 m_eyez,
              m_centerx,	m_centery,	 m_centerz,
              m_upx,	   	m_upy,		 m_upz);
    glScalef(1, 1, 1);
    glRotatef(x_rotate,1,0,0);
    glRotatef(y_rotate,0,1,0);
    glRotatef(z_rotate,0,0,1);
    glTranslatef(x_move, y_move, z_move);
    glScalef(m_zoom, m_zoom, m_zoom);
    Draw_Org();
    //std::cout<<"m_eyeyaw:"<<m_eyeyaw<<endl;
    DrawCar(m_eyex,m_eyey,m_eyeyaw,control_logic_->GetSteeringAngleFeedback());
    DrawCar_e(m_eyex, m_eyey,position_x_c, position_y_c,m_eyeyaw, yaw_c);
    if(debug_output_.path_.IsEmpty() == false)
    {
        Draw_Virtual_lane(debug_output_.path_, 100, debug_output_.path_.ref_points_.size(), 10,  0, 1, 0);
        Draw_Best_March_Point(debug_output_.path_,  debug_output_.match_point_num_);
    }

    glutPostRedisplay();
    usleep(10000);

    glFlush();
    glutSwapBuffers();
}

void ControlView::Draw_Virtual_lane(path& v_p,
                            int output_st_pos,
                            int output_en_pos,
                            int every,
                            int r, int g, int b)
{
    unsigned int i;
    double  direction;
    double direction_d;
    double cos_d;
    double sin_d;

    double x, xx, x11, x22, x33, x44;
    double y, yy, y11, y22, y33, y44;

    int length = v_p.ref_points_.size();

    if( length - every <= 0)
       return;

    for ( i = 0; i < v_p.ref_points_.size() - every; i+=every )
    {
        glLineWidth(1);
        glColor3d(r,g,b);

        direction = v_p.ref_points_[i].heading_;
        x = v_p.ref_points_[i].position_x_;
        y = v_p.ref_points_[i].position_y_;
        direction_d = to_radians(direction);
        cos_d = cos(direction_d);
        sin_d = sin(direction_d);

        x11 = x - vehicle_width_ /2.0 * cos_d;
        y11 = y + vehicle_width_ /2.0 * sin_d;
        x22 = x + vehicle_width_ /2.0 * cos_d;
        y22 = y - vehicle_width_ /2.0 * sin_d;

        direction = v_p.ref_points_[i+every].heading_;
        xx = v_p.ref_points_[i+every].position_x_;
        yy = v_p.ref_points_[i+every].position_y_;

        direction_d = to_radians(direction);
        cos_d = cos(direction_d);
        sin_d = sin(direction_d);

        x33 = xx - vehicle_width_ /2.0 * cos_d;
        y33 = yy + vehicle_width_ /2.0 * sin_d;
        x44 = xx + vehicle_width_ /2.0 * cos_d;
        y44 = yy - vehicle_width_ /2.0 * sin_d;

        glBegin(GL_LINE_STRIP);
        glVertex2f(x11,y11);
        glVertex2f(x22,y22);
        glVertex2f(x44,y44);
        glVertex2f(x33,y33);
        glVertex2f(x11,y11);
        glEnd();
    }
}

void ControlView::Draw_Best_March_Point(path& p,  int no_points)
{
    double x;
    double y;

    if(no_points <= 0)
    {
        return;
    }

    y = p.ref_points_[no_points].position_y_;
    x = p.ref_points_[no_points].position_x_;

    double xx = x;
    double yy = y;

    glPointSize(8);
    glColor3d(1, 1, 1);
    glBegin(GL_POINTS);
    glVertex2f(xx,yy);
    glEnd();

    glColor3d(1,1,1);
    glBegin(GL_LINES);
    glVertex2f(xx - 1, yy);
    glVertex2f(xx + 1, yy);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(xx, yy-1);
    glVertex2f(xx, yy+1);
    glEnd();
}


void ControlView::MyGLDispIni()
{
    GLenum type;

    m_eyex=0,  m_eyey=0,  m_eyez= 80;
    m_centerx=0,  m_centery=0, m_centerz=0;
    m_upx=0,  m_upy=1,  m_upz=0;

    buttonSaveLeft=false;
    buttonSaveMiddle=false;
    buttonSaveRight=false;

    x_move=0.0;
    y_move=0.0;
    z_move=0.0;
    x_rotate=0.0;
    y_rotate=0.0;
    z_rotate=0.0;
    m_zoom=1;

    x_lbefore=0, y_lbefore=0;
    x_rbefore=0, y_rbefore=0;
    z_before1=0, z_before2=0;

    type = GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE;
    glutInitDisplayMode(type);
    glutInitWindowSize(740, 860);

    string windows_name = "controller 2080802" ;
    glutCreateWindow(windows_name.c_str());

    glutReshapeFunc( Reshape );
    glutKeyboardFunc(Key);
    glutSpecialFunc(SpecialKey);
    glutMouseFunc(MouseKey);
    glutMotionFunc(MouseMove);
    glutPassiveMotionFunc(PassiveMouseMove);
    glutSpaceballRotateFunc(MouseRotate);

    glutDisplayFunc(&myDisplay);
}

void ControlView::Init(ControlLogic*control_logic,double vehicle_width)
{
    vehicle_width_ = vehicle_width;
    control_logic_ = control_logic;
}

void ControlView::Draw_Org()
{
    glPointSize(8);
    glColor3d(1, 1, 1);
    glBegin(GL_POINTS);
    glVertex2f(0, 0);
    glEnd();
    //
    glColor3d(1,1,1);
    glBegin(GL_LINES);
    glVertex2f(0 - 2, 0);
    glVertex2f(0 + 2, 0);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(0, 0-3);
    glVertex2f(0, 0+3);
    glEnd();
}

void ControlView::Draw_Point(double x, double y)
{
    glPointSize(3);
//   glColor3d(0, 0, 1);
    glBegin(GL_POINTS);
    glVertex2f(0, 0);
    glEnd();
    //
//    glColor3d(0,0,1);
    glBegin(GL_LINES);
    glVertex2f(x - 0.5, y);
    glVertex2f(x + 0.5, y);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(x, y - 0.5);
    glVertex2f(x, y + 0.5);
    glEnd();
}


void ControlView::DrawCar_e(double x, double y,  double x_c, double  y_c, double yaw,  double yaw_c)
{
    glLineWidth(2);
    glColor3d(0,1,0);
    glBegin(GL_LINES);
    glVertex2f(x, y);
    glVertex2f(x_c, y_c);
    glEnd();
}

void ControlView::DrawCar(double x, double y,  double yaw, double steer_angle)
{
    glPointSize(8);
    glColor3d(1,0,0);
    glBegin(GL_POINTS);
    glVertex2f( x + (1.2) * sin( yaw /180 *PI ),
                y + (1.2) * cos( yaw /180 *PI ) );
    glEnd();

    double frontwtht = to_radians(yaw);
    // double backwtht = to_radians(yaw + 90);
    double wheel_direction = to_radians(yaw + steer_angle/15 + 90); //

    double frontWhellCenterX = x + (1.2) * sin(frontwtht);
    double frontWhellCenterY = y + (1.2) * cos(frontwtht);
    double backWhellCenterX = x - (1.5) * sin(frontwtht);
    double backWhellCenterY = y - (1.5) * cos(frontwtht);

    glLineWidth(2);
    glColor3d(0,1,0);
    glBegin(GL_LINES);
    glVertex2f(x, y);
    glVertex2f(frontWhellCenterX,frontWhellCenterY );
    glEnd();

    glColor3d(1,0,0);
    glBegin(GL_LINES);
    glVertex2f(x, y);
    glVertex2f(backWhellCenterX,backWhellCenterY );
    glEnd();

    double frontWhellLeftX = frontWhellCenterX - (( front_track/2.0) * cos( frontwtht));
    double frontWhellLeftY = frontWhellCenterY + (( front_track/2.0) * sin( frontwtht));
    double frontWhellRightX =  frontWhellCenterX + (( front_track/2.0) * cos( frontwtht));
    double frontWhellRightY =  frontWhellCenterY - (( front_track/2.0) * sin( frontwtht));

    glLineWidth(2);
    glColor3d(1,0,1);

    glBegin(GL_LINES);
    glVertex2f(frontWhellLeftX, frontWhellLeftY);
    glVertex2f(frontWhellRightX, frontWhellRightY);
    glEnd();

    double frontLeftWhellUpX = frontWhellLeftX - (( front_wheel_wide/2.0) * cos( wheel_direction ));
    double frontLeftWhellUpY = frontWhellLeftY + (( front_wheel_wide/2.0) * sin( wheel_direction ));
    double frontLeftWhellDownX = frontWhellLeftX + (( front_wheel_wide/2.0) * cos( wheel_direction ));
    double frontLeftWhellDownY = frontWhellLeftY - (( front_wheel_wide/2.0) * sin( wheel_direction ));

    glLineWidth(2);
    glColor3d(1,0,1);

    glBegin(GL_LINES);
    glVertex2f(frontLeftWhellUpX, frontLeftWhellUpY);
    glVertex2f(frontLeftWhellDownX, frontLeftWhellDownY);
    glEnd();

    double frontrightWhellUpX = frontWhellRightX - (( front_wheel_wide/2.0) * cos( wheel_direction ));
    double frontrightWhellUpY = frontWhellRightY + (( front_wheel_wide/2.0) * sin( wheel_direction ));
    double frontrightWhellDownX = frontWhellRightX + (( front_wheel_wide/2.0) * cos( wheel_direction ));
    double frontrightWhellDownY = frontWhellRightY - (( front_wheel_wide/2.0) * sin( wheel_direction ));

    glLineWidth(2);
    glColor3d(1,0,1);

    glBegin(GL_LINES);
    glVertex2f(frontrightWhellUpX, frontrightWhellUpY);
    glVertex2f(frontrightWhellDownX, frontrightWhellDownY);
    glEnd();

    double BackWhellLeftX = backWhellCenterX - (( front_track/2.0) * cos( frontwtht));
    double BackWhellLeftY = backWhellCenterY + (( front_track/2.0) * sin( frontwtht));
    double BackWhellRightX =  backWhellCenterX + (( front_track/2.0) * cos( frontwtht));
    double BackWhellRightY =  backWhellCenterY - (( front_track/2.0) * sin( frontwtht));

    double backwtht = to_radians(yaw + 90);

    glLineWidth(2);
    glColor3d(1,0,1);

    glBegin(GL_LINES);
    glVertex2f(BackWhellRightX, BackWhellRightY);
    glVertex2f(BackWhellLeftX, BackWhellLeftY);
    glEnd();

    double BackLeftWhellUpX = BackWhellLeftX - ((back_wheel_wide/2.0) * cos(backwtht));
    double BackLeftWhellUpY = BackWhellLeftY + (( back_wheel_wide/2.0) * sin(backwtht));
    double BackLeftWhellDownX = BackWhellLeftX + (( back_wheel_wide/2.0) * cos(backwtht));
    double BackLeftWhellDownY = BackWhellLeftY - (( back_wheel_wide/2.0) * sin(backwtht));

    glLineWidth(2);
    glColor3d(1,0,1);

    glBegin(GL_LINES);
    glVertex2f(BackLeftWhellDownX, BackLeftWhellDownY);
    glVertex2f(BackLeftWhellUpX, BackLeftWhellUpY);
    glEnd();

    double BackrightWhellUpX = BackWhellRightX - ((back_wheel_wide/2.0) * cos(backwtht));
    double BackrightWhellUpY = BackWhellRightY + ((back_wheel_wide/2.0) * sin(backwtht));
    double BackrightWhellDownX = BackWhellRightX + ((back_wheel_wide/2.0) * cos(backwtht));
    double BackrightWhellDownY = BackWhellRightY - ((back_wheel_wide/2.0) * sin(backwtht));

    glLineWidth(2);
    glColor3d(1,0,1);

    glBegin(GL_LINES);
    glVertex2f(BackrightWhellUpX, BackrightWhellUpY);
    glVertex2f(BackrightWhellDownX, BackrightWhellDownY);
    glEnd();
}


}
}
