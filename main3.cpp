//
// Created by 86155 on 2023/11/30.
//
// test00.cpp : Defines the entry point for the console application.
//


#include <GL/glut.h>

void myDisplay();
void myKeyboard(unsigned char key, int x, int y);
void myMouse(GLint button, GLint  state, GLint x, GLint y);
void myMouseMotion(int x, int y);

void myReshape(GLint w, GLint h);


struct mycolor
{float R;float G;float B;};

float eye[3]={0.0,0.0,3.0};
float center[3]={0.0,0.0,0.0};

struct mycolor MyColor;

int mouseY;
bool mouseLeftDown=false;


int main(int argc, char** argv)
{

    MyColor.R=1.0; MyColor.G=1.0; MyColor.B=1.0;
    glutInit(&argc, argv);
    glutCreateWindow("bnojk");
    glutDisplayFunc(myDisplay);
    glutKeyboardFunc(&myKeyboard);
    glutMouseFunc(&myMouse);
    glutMotionFunc(&myMouseMotion);
    glutReshapeFunc(&myReshape);
    glutMainLoop();
}




void myDisplay()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(	MyColor.R, MyColor.G, MyColor.B);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye[0], eye[1], eye[2],center[0], center[1],center[2], 0, 1, 0);

    glutWireDodecahedron();
    glFlush();
}




void myKeyboard(unsigned char key, int x, int y)
{


    switch (key)
    {
        case'R':
        case 'r':
            MyColor.R=1.0;
            MyColor.G=0.0;
            MyColor.B=0.0;
            break;

        case'G':
        case 'g':
            MyColor.R=0.0;
            MyColor.G=1.0;
            MyColor.B=0.0;
            break;

        case'B':
        case 'b':
            MyColor.R=0.0;
            MyColor.G=0.0;
            MyColor.B=1.0;
            break;
        case'Q':
        case 'q':
            exit(0);
    }

    myDisplay();

}


void myMouse(GLint button, GLint  state, GLint x, GLint y)
{
    if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN)
    {
        mouseLeftDown=true;
        mouseY = y;
        MyColor.R = 1.0;
        MyColor.G = 1.0;
        MyColor.B = 0.0;
    }

    if(button==GLUT_LEFT_BUTTON&&state==GLUT_UP)
        mouseLeftDown=false;


    myDisplay();
}

void myMouseMotion(int x, int y)
{

    if (mouseLeftDown) eye[2] += (y - mouseY)*0.1;
    mouseY = y;

    myDisplay();
}

void myReshape(GLint w, GLint h)
{

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(33,(float)w/(float)h, 1,100);


}