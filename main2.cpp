//
// Created by 86155 on 2023/11/23.
//
#include <GL/glut.h>

int shape = 1; // 0表示线段，1表示三角形，2表示四边形，3表示多边形
float colorRed = 1.0, colorGreen = 1.0, colorBlue = 1.0; // 初始化颜色为白色
double posX = 0, posY = 0; // 初始化图形位置为原点

void drawShape() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(colorRed, colorGreen, colorBlue);
    if (shape == 0) {
        glBegin(GL_LINES);
        glVertex2f(-0.5 + posX, 0.0 + posY);
        glVertex2f(0.5 + posX, 0.0 + posY);
        glEnd();
    } else if (shape == 1) {
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5 + posX, -0.5 + posY);
        glVertex2f(0.5 + posX, -0.5 + posY);
        glVertex2f(0.0 + posX, 0.5 + posY);
        glEnd();
    } else if (shape == 2) {
        glBegin(GL_QUADS);
        glVertex2f(-0.5 + posX, -0.5 + posY);
        glVertex2f(0.5 + posX, -0.5 + posY);
        glVertex2f(0.5 + posX, 0.5 + posY);
        glVertex2f(-0.5 + posX, 0.5 + posY);
        glEnd();
    } else if (shape == 3) {
        glBegin(GL_POLYGON);
        glVertex2f(-0.5 + posX, -0.5 + posY);
        glVertex2f(0.5 + posX, -0.5 + posY);
        glVertex2f(0.7 + posX, 0.0 + posY);
        glVertex2f(0.5 + posX, 0.5 + posY);
        glVertex2f(-0.5 + posX, 0.5 + posY);
        glEnd();
    }
    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 't') {
        shape = 1;
    } else if (key == 's') {
        shape = 2;
    } else if (key == 'd') {
        shape = 3;
    } else if (key == 'l') {
        shape = 0;
    } else if (key == 'r') {
        colorRed = 1.0; colorGreen = 0.0; colorBlue = 0.0; // 红色
    } else if (key == 'g') {
        colorRed = 0.0; colorGreen = 1.0; colorBlue = 0.0; // 绿色
    } else if (key == 'b') {
        colorRed = 0.0; colorGreen = 0.0; colorBlue = 1.0; // 蓝色
    }
    drawShape();
}

void specialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {
        posY += 0.3;
    } else if (key == GLUT_KEY_DOWN) {
        posY -= 0.3;
    } else if (key == GLUT_KEY_LEFT) {
        posX -= 0.3;
    } else if (key == GLUT_KEY_RIGHT) {
        posX += 0.3;
    }
    drawShape();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        shape = 2; // 左键点击改为四边形
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        colorRed = 1.0; colorGreen = 0.5; colorBlue = 0.0; // 右键点击改为橙色
    }
    drawShape();
}

void myreshape(GLint w, GLint h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(w<=h)
        gluOrtho2D(-2.0, 2.0, -2.0*(GLfloat)h/(GLfloat)w,
                   2.0*(GLfloat)h/(GLfloat)w);
    else gluOrtho2D(-2.0*(GLfloat)w/(GLfloat)h,
                    2.0*(GLfloat)w/(GLfloat)h, -2.0, 2.0);
    glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutCreateWindow("Interactive Program");
    glutInitWindowSize(400, 400);
    glutDisplayFunc(drawShape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouse);
    glutReshapeFunc(myreshape);
    glOrtho(-1, 1, -1, 1, -1, 1);
    glutMainLoop();
    return 0;
}