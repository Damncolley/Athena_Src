#include <math.h>

#include <GL/glu.h>
#include <GL/glut.h>


#include "draw_obu_planning.h"
#include "obu_planning.h"


int x_lbefore,y_lbefore;
int x_rbefore,y_rbefore;
int z_before1,z_before2;

bool buttonSaveLeft,  buttonSaveMiddle,  buttonSaveRight;
float x_move,y_move,z_move;
float x_move_save,y_move_save, z_move_save;
float x_rotate,y_rotate,z_rotate;
float x_rotate_save,y_rotate_save,z_rotate_save;
float m_zoom;

float m_aspect;

float m_eyex, m_eyey,  m_eyez;
float m_centerx,  m_centery, m_centerz;
float m_upx, m_upy, m_upz;

///////////////////OPEN GL control ///////////////////////////////////////
int g_frame;
bool g_pause;

void OpenGL_Draw()
{
    x_move = 0,y_move = 0,z_move = 0;
    x_rotate =1,y_rotate=1,z_rotate=1;
    m_zoom=1;
    g_frame=0;
}

void Reshape(int w, int h)
{
    glViewport(0, 0, (GLint)w, (GLint)h);

    m_aspect = (GLfloat) w / (GLfloat) h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f,
                   m_aspect,
                   0.0f,
                   400.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void MouseMove(int x, int y)
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
        float multiplay = (y - z_move_save)/10000;
        m_zoom =m_zoom*(1+multiplay);
    }

    if(buttonSaveRight)
    {
        float multiplay = (y - z_move_save)/10000;
        m_zoom =m_zoom*(1+multiplay);
    }
}

void PassiveMouseMove(int x, int y)
{

}

void MouseRotate(int x, int y, int z)
{
}

void MouseKey(int button, int state, int x, int y)
{
    x_move_save=x;
  //  y_move_save;
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

void Key(unsigned char key, int x, int y)
{
    switch (key)
    {
    case KEY_ESC:
      //  exit(0);
        break;
    }
}

void SpecialKey(int key, int x, int y)
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
        x_rotate += 3;
        if (x_rotate > 360)
            x_rotate=x_rotate - 360;
        if (x_rotate < -360)
            x_rotate=x_rotate + 360;
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
        break;

    case GLUT_KEY_F11:
        break;

    case GLUT_KEY_F12:
        break;
    }

    glutPostRedisplay();
}

/*
    g_rsu_planning->origin_x  = 27749.4;
    g_rsu_planning->origin_y = -10034.9;
    g_rsu_planning->origin_z = 0;
*/

void Draw_Org()
{
    double x =  27749.4;
    double y = -10034.9;

    glPointSize(8);
    glColor3d(1, 1, 1);
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
    //
    glColor3d(1,1,1);
    glBegin(GL_LINES);
    glVertex2f(x - 2, y);
    glVertex2f(x + 2, y);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(x, y-3);
    glVertex2f(x, y+3);
    glEnd();
}

void draw_section_line(route::RouteBase route)
{
    if (route.route_motion_info_.num_of_section == 0 )
    {
        return;
    }

    for(int i = 0; i < route.route_motion_info_.num_of_section; i++)
    {
        for(int j = 0; j < route.route_motion_info_.line[i].num_of_lane-1; j++)
        {

            //画点
            /*
            glPointSize(2);
            glColor3d(0, 3, 3);
            glBegin(GL_POINTS);
            glVertex2f(route.route_motion_info_.line[i].lane[j].x, route.route_motion_info_.line[i].lane[j].y);
            glEnd();
            */

            //画线
            /*
            glLineWidth(1);
            glColor3d(3, 3, 3);
            glBegin(GL_LINE_STRIP);
            glVertex2f(route.route_motion_info_.line[i].lane[j].x, route.route_motion_info_.line[i].lane[j].y);
            glVertex2f(route.route_motion_info_.line[i].lane[j+1].x, route.route_motion_info_.line[i].lane[j+1].y);
            glEnd();*/
        }
    }
    glutPostRedisplay();           //武汉经济技术开发区出口加工区A栋
}



void draw_obu_planning()
{

    Draw_Org();

    //draw lanelet and centerline
    if(g_obu_planning->obu.route_draw_flag == true)
    {
        if(g_obu_planning->obu.route_draw[1] == NULL) return;
        if(g_obu_planning->obu.route_draw[1]->map_->lane_list_.size() > 0)
        {
            //g_obu_planning->obu.route_draw[1].draw();
            g_obu_planning->obu.route_draw[1]->draw_part_line();
            double x,y;
            g_obu_planning->obu.route_draw[1]->transfer_.cs.ll2xy(g_obu_planning->obu.cur_lat, g_obu_planning->obu.cur_lon, x, y);
            DrawCar(x, y, g_obu_planning->obu.cur_yaw, 0) ;
        }
    }
    else
    {
        if(g_obu_planning->obu.route_draw[0]->map_->lane_list_.size() > 0)
        {
            //g_obu_planning->obu.route_draw[0].draw();
            g_obu_planning->obu.route_draw[0]->draw_part_line();
            double x,y;
            g_obu_planning->obu.route_draw[0]->transfer_.cs.ll2xy(g_obu_planning->obu.cur_lat, g_obu_planning->obu.cur_lon, x, y);
            DrawCar(x, y, g_obu_planning->obu.cur_yaw, 0) ;
        }
    }

    glutPostRedisplay();
}

void myDisplay(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glLoadIdentity();

  //  x_rotate = -102, y_rotate = -3 , z_rotate = 57.4561 ;
  //  x_move = 156.06, y_move = -166, z_move =0, m_zoom = 2.14359;
    double x,y;
    g_obu_planning->obu.route->transfer_.cs.ll2xy(g_obu_planning->obu.cur_lat, g_obu_planning->obu.cur_lon, x, y);

    m_eyex = x;
    m_eyey = y;
    m_eyez = 300;

    m_centerx = x ;
    m_centery = y;
    m_centerz = 0;

    m_upx = x;
    m_upy = y +10;
    m_upz = 0;

    gluLookAt(m_eyex, 	  m_eyey, 	 m_eyez,
              m_centerx,	m_centery,	 m_centerz,
              m_upx,	   	m_upy,		 m_upz);

    glScalef(1, 1, 1);

    glRotatef(x_rotate,1,0,0);
    glRotatef(y_rotate,0,1,0);
    glRotatef(z_rotate,0,0,1);

    glTranslatef(x_move, y_move, z_move);
    glScalef(m_zoom, m_zoom, m_zoom);

    draw_obu_planning();

    glFlush();
    glutSwapBuffers();
}


void MyGLDispIni()
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

    glutInitWindowSize(600, 800);
    glutCreateWindow("obu_planning");

    glutReshapeFunc( Reshape );
    glutKeyboardFunc(Key);
    glutSpecialFunc(SpecialKey);
    glutMouseFunc(MouseKey);
    glutMotionFunc(MouseMove);
    glutPassiveMotionFunc(PassiveMouseMove);
    glutSpaceballRotateFunc(MouseRotate);

    glutDisplayFunc( &myDisplay);
}

void DrawCar(double x, double y,  double yaw, double steer_angle)
{
    double front_track= 1.2;
    double front_wheel_wide = 1;
    double back_wheel_wide = 1;

    glPointSize(8);
    glColor3d(1,0,0);
    glBegin(GL_POINTS);
    glVertex2f( x + (1.2) * sin( yaw /180 *PI ),
                y + (1.2) * cos( yaw /180 *PI ) );
    glEnd();

    double frontwtht = to_radians(yaw);
    // double backwtht = to_radians(yaw + 90);
    double wheel_direction = to_radians(yaw + steer_angle /15.0 + 90); //

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
