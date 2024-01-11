//
// Created by 86155 on 2023/12/24.
//
#include<gl/glut.h>
GLuint textureID;

void loadTexture(const char* filename) {
    // ���ļ��м�������ͼƬ��Ȼ���������ݴ��ݸ��������
    // ����ʹ��SOIL��������ͼƬ
    int width, height;
    unsigned char* image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    // �����������
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SOIL_free_image_data(image);
}

void display() {
    // ����һ�����Σ���������������
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -1.0, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.0, 0.0);
    glEnd();

    glFlush();
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutCreateWindow("Texture Example");.
    

    loadTexture("texture.jpg");

    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
