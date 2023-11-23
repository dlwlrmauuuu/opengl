
#include<GL/glut.h>


void mydisplay() {

    glBegin(GL_QUADS);
    glVertex2f(-0.5, -0.5);
    glVertex2f(-0.5, 0.5);
    glVertex2f(0.5, 0.5);
    glVertex2f(0.5, -0.5);
    glEnd();
    glFlush();
}
void mydisplay2() {
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
    glColor3f(1.0f,0.0f,0.0f);
    glVertex3f(0.0f, 0.5f,0.0f);
    glColor3f(0.0f,1.0f,0.0f);
    glVertex3f(-0.5f, -0.5f,0.0f);
    glColor3f(0.0f,0.0f,1.0f);
    glVertex3f(0.5f, -0.5f,0.0f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(1.5f, 0.5f);
    glVertex2f(2.0f, 0.5f);
    glVertex2f(1.5f, -0.5f);
    glVertex2f(2.0f, -0.5f);
    glEnd();

    glFlush();


}
int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Multiple Shapes");

    glutDisplayFunc(mydisplay2);

    glutMainLoop();
}