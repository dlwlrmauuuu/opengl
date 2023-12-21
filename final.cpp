

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
GLfloat rotationX = 0.0;
GLfloat rotationY = 0.0;
GLfloat rotationZ = 0.0;

void init(void) {
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 100.0 };

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glEnable(GL_DEPTH_TEST);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(rotationX, 1.0, 0.0, 0.0);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glRotatef(rotationZ, 0.0, 0.0, 1.0);

    // Draw the classroom here
    // Draw the blackboard
    glPushMatrix();
    glTranslatef(0.0, 0.0, -5.0);
    glScalef(2.0, 1.0, 0.1);
    glutSolidCube(1.0);
    glPopMatrix();

    // Draw the chair
    glPushMatrix();
    glTranslatef(-2.0, -1.5, -2.0);
    glutSolidCube(0.5);
    glTranslatef(0.0, 0.0, 4.0);
    glutSolidCube(0.5);
    glPopMatrix();

    // Draw the table
    glPushMatrix();
    glTranslatef(2.0, -1.5, 0.0);
    glScalef(1.5, 0.1, 1.0);
    glutSolidCube(1.0);
    glPopMatrix();

    // Draw the student
    glPushMatrix();
    glTranslatef(1.0, -1.0, -2.0);
    glutSolidSphere(0.3, 20, 20);
    glPopMatrix();

    glFlush();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'x':
            rotationX += 5.0;
            break;
        case 'X':
            rotationX -= 5.0;
            break;
        case 'y':
            rotationY += 5.0;
            break;
        case 'Y':
            rotationY -= 5.0;
            break;
        case 'z':
            rotationZ += 5.0;
            break;
        case 'Z':
            rotationZ -= 5.0;
            break;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        rotationX = 0.0;
        rotationY = 0.0;
        rotationZ = 0.0;
        glutPostRedisplay();
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Classroom");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}