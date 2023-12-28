#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdio>


GLubyte* image;
int width, height;
GLuint texture;

GLfloat light_position0[] = { -1.0, 1.0, 1.0, 0.0 };
GLfloat light_position1[] = { 1.0, -1.0, 1.0, 0.0 };
GLfloat light_position2[] = { 0.0, 0.0, -1.0, 0.0 };

GLfloat teapotPos[] = { 0.0, 0.0, 0.0 };
GLfloat teapotRot[] = { 0.0, 0.0, 0.0 };

void loadTexture() {
    FILE *file;
    char filename[] = "texture.bmp";
    file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error: Unable to open the texture file.\n");
        exit(1);
    }
    width = 256;
    height = 256;
    image = (GLubyte*)malloc(width * height * 3);
    fread(image, width * height * 3, 1, file);
    fclose(file);
}

void init(void) {
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    loadTexture();
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glShadeModel(GL_SMOOTH);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    GLfloat light_diffuse0[] = { 1.0, 0.0, 0.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
    GLfloat light_diffuse1[] = { 0.0, 1.0, 0.0, 1.0 };
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
    GLfloat light_diffuse2[] = { 0.0, 0.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glTranslatef(teapotPos[0], teapotPos[1], teapotPos[2]);
    glRotatef(teapotRot[0], 1.0, 0.0, 0.0);
    glRotatef(teapotRot[1], 0.0, 1.0, 0.0);
    glRotatef(teapotRot[2], 0.0, 0.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glutSolidTeapot(1.0);
    glPopMatrix();
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
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        teapotPos[0] = 0.0;
        teapotPos[1] = 0.0;
        teapotPos[2] = 0.0;
        teapotRot[0] = 0.0;
        teapotRot[1] = 0.0;
        teapotRot[2] = 0.0;
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
        case 'w':
            teapotPos[2] -= 1.0;
            break;
        case 's':
            teapotPos[2] += 1.0;
            break;
        case 'a':
            teapotPos[0] -= 1.0;
            break;
        case 'd':
            teapotPos[0] += 1.0;
            break;
        case 'i':
            teapotRot[0] -= 5.0;
            break;
        case 'k':
            teapotRot[0] += 5.0;
            break;
        case 'j':
            teapotRot[1] -= 5.0;
            break;
        case 'l':
            teapotRot[1] += 5.0;
            break;
        case 'u':
            teapotRot[2] -= 5.0;
            break;
        case 'o':
            teapotRot[2] += 5.0;
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
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}