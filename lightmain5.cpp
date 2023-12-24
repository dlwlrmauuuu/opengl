#include <GL/glut.h>

GLfloat light_position0[] = { -1.0, 1.0, 1.0, 0.0 };
GLfloat light_position1[] = { 1.0, -1.0, 1.0, 0.0 };
GLfloat light_position2[] = { 0.0, 0.0, -1.0, 0.0 };

void init(void) {
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };

    glClearColor(0.0, 0.0, 0.0, 0.0);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv(GL_LIGHT2, GL_POSITION, light_position2);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutSolidTeapot(1.0);
    glFlush();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        static int lightSource = 0;
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT2);
        glEnable(GL_LIGHT0 + lightSource);
        lightSource = (lightSource + 1) % 3;
    }
    glutPostRedisplay();
}
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case '1':
            glDisable(GL_LIGHT0);
            glDisable(GL_LIGHT1);
            glDisable(GL_LIGHT2);
            glEnable(GL_LIGHT0);
            break;
        case '2':
            glDisable(GL_LIGHT0);
            glDisable(GL_LIGHT1);
            glDisable(GL_LIGHT2);
            glEnable(GL_LIGHT1);
            break;
        case '3':
            glDisable(GL_LIGHT0);
            glDisable(GL_LIGHT1);
            glDisable(GL_LIGHT2);
            glEnable(GL_LIGHT2);
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse); // ÐÞ¸ÄÊó±ê²Ù×÷
    glutMainLoop();
    return 0;
}