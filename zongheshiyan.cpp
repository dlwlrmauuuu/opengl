#include<gl/glut.h>
#include<math.h>
#include <stdio.h>
#include <stdlib.h>
//������������õĳ�������
GLfloat light_position1[]={0,28,-20,1.0};
GLfloat model_ambient[]={0.05f,0.05f,0.05f,1.0f};
GLfloat mat_specular[]={0.8,1.0,1.0,1.0};
GLfloat mat_shininess[]={5.0};
GLfloat mat_ambient[]={0.1,0.1,0.1,1};
GLfloat white_light[]={1.0,1.0,1.0,1.0};
GLfloat light[]={1.0,1.0,1.0,1};
GLfloat light_position0[]={0,28,20,1.0};
GLfloat	no_mat[]= {0.0f, 0.0f, 0.0f, 1.0f};		
GLfloat	mat_diffuse1[]= {0.1f, 0.5f, 0.8f, 1.0f};	
GLfloat	no_shininess[]= {0.0f};
GLint	WinWidth;
GLint	WinHeight;

//�����ӵ�ṹ
typedef struct EyePoint		
{
	GLfloat	x;
	GLfloat y;
	GLfloat z;
}EyePoint;
EyePoint 	myEye;
EyePoint    vPoint;
GLfloat vAngle=0;
//�����������غ���
#define BMP_Header_Length 54

//�ж�һ�����Ƿ���2�������η�
int power_of_two(int n)
{
	if( n <= 0 )
		return 0;
    return (n & (n-1)) == 0;
}
//����һ��λͼ��Ϊ�������ص���������
GLuint load_texture(const char* file_name)
{
    GLint width, height, total_bytes; 
	GLubyte* pixels = 0;
	GLint last_texture_ID=0;
	GLuint texture_ID = 0;
// ���ļ������ʧ�ܣ�����
    FILE* pFile = fopen(file_name, "rb");
	if( pFile == 0 )
	return 0;
// ��ȡ�ļ���ͼ��Ŀ�Ⱥ͸߶�
	fseek(pFile, 0x0012, SEEK_SET);
	fread(&width, 4, 1, pFile);
	fread(&height, 4, 1, pFile);
	fseek(pFile, BMP_Header_Length, SEEK_SET);
// ����ÿ��������ռ�ֽ����������ݴ����ݼ����������ֽ���
{
    GLint line_bytes = width * 3;
	while( line_bytes % 4 != 0 )
	++line_bytes;
	total_bytes = line_bytes * height;
}
// �����������ֽ��������ڴ�
    pixels = (GLubyte*)malloc(total_bytes);
	if( pixels == 0 )
	{
		fclose(pFile);
		return 0;
	}
// ��ȡ��������
	if( fread(pixels, total_bytes, 1, pFile) <= 0 )
	{
		free(pixels); 
		fclose(pFile); 
		return 0;
	}
{
	GLint max;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
	if( !power_of_two(width)|| !power_of_two(height)|| width > max|| height > max )
	{   
		const GLint new_width = 256;
		const GLint new_height = 256; // �涨���ź��µĴ�СΪ�߳���������
        GLint new_line_bytes, new_total_bytes; 
		GLubyte* new_pixels = 0;
 // ����ÿ����Ҫ���ֽ��������ֽ���
		new_line_bytes = new_width * 3;
		while( new_line_bytes % 4 != 0 )
		++new_line_bytes;
		new_total_bytes = new_line_bytes * new_height;
// �����ڴ�
		new_pixels = (GLubyte*)malloc(new_total_bytes);
		if( new_pixels == 0 )
		{
			free(pixels); 
			fclose(pFile); 
			return 0;
		}
// ������������
		gluScaleImage(GL_RGB,width, height, GL_UNSIGNED_BYTE, pixels,new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);
// �ͷ�ԭ�����������ݣ��� pixels ָ���µ��������ݣ����������� width �� height
		free(pixels);
		pixels = new_pixels; 
		width = new_width; 
		height = new_height;
	}
}
// ����һ���µ�������
		glGenTextures(1, &texture_ID);
		if( texture_ID == 0 )
		{
			free(pixels); 
			fclose(pFile); 
			return 0;
		}
// ���µ������������������������
// �ڰ�ǰ���Ȼ��ԭ���󶨵������ţ��Ա��������лָ�
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture_ID); 
		glBindTexture(GL_TEXTURE_2D, texture_ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
		glBindTexture(GL_TEXTURE_2D, last_texture_ID);
// ֮ǰΪ pixels ������ڴ����ʹ�� glTexImage2D �Ժ��ͷ�
// ��Ϊ��ʱ���������Ѿ��� OpenGL ���б�����һ�ݣ����ܱ����浽ר�ŵ�ͼ��Ӳ���У�
		free(pixels);
		return texture_ID;
}
GLuint texblackboard,texwindow, picture;
GLuint texceiling,texdoor,texbackwall;
GLuint texgaodi,texsdesk,texclock;

//���ƽ�������󳡾�
void drawscence()
{
	//���ò�����ز���
	glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
	glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);   

	//���Ȼ����컨��
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texceiling);
	glColor3f(0.3,0.3,0.3);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);	                  //���ڶ��巨������
    glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-40.0f,30.0f, 30.0f); 
	glTexCoord2f(0.0f, 3.0f);
	glVertex3f(-40.0f, 30.0f, -30.0f); 
	glTexCoord2f(6.0f, 3.0f);
	glVertex3f(40.0f, 30.0f, -30.0f); 
	glTexCoord2f(6.0f, 0.0f);
	glVertex3f(40.0f, 30.0f, 30.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	//���Ƶذ�
    glColor3f(0.8f, 1.0f, 0.8f);         
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);	                  //���ڶ��巨������
	glVertex3f(-40.0f,0.0f, 30.0f); 
	glVertex3f(-40.0f, 0.0f, -30.0f); 
	glVertex3f(40.0f, 0.0f, -30.0f); 
	glVertex3f(40.0f, 0.0f, 30.0f);
	glEnd();
	//�������ǽ����ߴ���
	glColor3f(0.8f,0.8f, 0.8f);
	glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);	                  //���ڶ��巨������
    glVertex3f(-40.0f,0.0f, 30.0f); 
    glVertex3f(-40.0f, 30.0f, 30.0f); 
    glVertex3f(-40.0f, 30.0f, -30.0f); 
    glVertex3f(-40.0f, 0.0f, -30.0f);
    glEnd();
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texwindow);
	for(int n=0;n<=1;n++)
	{
		glBegin(GL_QUADS);
        glNormal3f(1.0, 0.0f, 0.0f);	                  //���ڶ��巨������
        glTexCoord2f(1.0f, 0.0f);glVertex3f(-39.9, 10, -8+n*18); 
		glTexCoord2f(1.0f, 1.0f);glVertex3f(-39.9, 20, -8+n*18); 
		glTexCoord2f(0.0f, 1.0f);glVertex3f(-39.9, 20, -18+n*18); 
		glTexCoord2f(0.0f, 0.0f);glVertex3f(-39.9, 10, -18+n*18);
		glEnd();
		}
		glDisable(GL_TEXTURE_2D);
		//�����ұ�ǽ���ұ�
		glColor3f(0.8f,0.8f, 0.8f);
        glBegin(GL_QUADS);
        glNormal3f(-1.0f, 0.0f, 0.0f); //���ڶ��巨������
		glVertex3f(40.0f,0.0f, 30.0f); 
		glVertex3f(40.0f, 30.0f, 30.0f); 
		glVertex3f(40.0f, 30.0f, -30.0f); 
		glVertex3f(40.0f, 0.0f, -30.0f);
		glEnd();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, picture);
        glBegin(GL_QUADS);
        glNormal3f(-1.0, 0.0f, 0.0f);	                  //���ڶ��巨������
        glTexCoord2f(1.0f, 0.0f);glVertex3f(39.5, 10, 10);
		glTexCoord2f(1.0f, 1.0f);glVertex3f(39.5, 20, 10);
		glTexCoord2f(0.0f, 1.0f);glVertex3f(39.5, 20, 0);
		glTexCoord2f(0.0f, 0.0f);glVertex3f(39.5, 10, 0);
		glEnd();
		glDisable(GL_TEXTURE_2D);
        //���ƺ��ǽ
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texbackwall);
        glColor3f(0.8f,0.8f, 0.8f);
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f); //���ڶ��巨������
        glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-40.0f,0.0f, 30.0f); 
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-40.0f, 30.0f, 30.0f); 
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(40.0f, 30.0f, 30.0f); 
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(40.0f, 0.0f, 30.0f);
		glEnd();
	    //����ǰ��ǽ
        glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texbackwall);
        glColor3f(0.8f,0.8f, 0.8f);
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f); //���ڶ��巨������
        glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-40.0f,0.0f, -30.0f); 
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-40.0f, 30.0f, -30.0f); 
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(40.0f, 30.0f, -30.0f); 
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(40.0f, 0.0f, -30.0f);
		glEnd();
        //����
		glBindTexture(GL_TEXTURE_2D, texclock);
		glBegin(GL_QUADS); 
        glNormal3f(0.0f, 0.0f, 1.0f); //���ڶ��巨������
        glTexCoord2f(0.0f, 0.0f);
		glVertex3f(23.0f,18.0f, -29.8f); 
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(23.0f, 20.0f, -29.8f); 
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(25.0f, 20.0f, -29.8f); 
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(25.0f, 18.0f, -29.8f);
		glEnd();
		glDisable(GL_TEXTURE_2D);
            //���ƺڰ�
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texblackboard);
			glBegin(GL_QUADS);
			glNormal3f(0.0f, 0.0f, 1.0f); //���ڶ��巨������
			glTexCoord2f(0.0f, 0.0f);glVertex3f(-20.0,8.0f, -29.9f); 
			glTexCoord2f(0.0f, 1.0f);glVertex3f(-20.0, 18.0f, -29.9f); 
			glTexCoord2f(1.0f, 1.0f);glVertex3f(20.0, 18.0f, -29.9f); 
			glTexCoord2f(1.0f, 0.0f);glVertex3f(20.0, 8.0f, -29.9f);
			glEnd();
			glDisable(GL_TEXTURE_2D);

			//���ƽ���ǰ��һ��ߵز�������
            glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texgaodi);
			//������
			glBegin(GL_QUADS);
			glNormal3f(0.0f, 1.0f, 0.0f); //���ڶ��巨������
			glTexCoord2f(0.0f, 0.0f);glVertex3f(-30.0f, 1.5f, -22.0f); 
			glTexCoord2f(0.0f, 1.0f);glVertex3f(-30.0f, 1.5f, -30.0f); 
			glTexCoord2f(1.0f, 1.0f);glVertex3f(30.0f, 1.5f, -30.0f); 
			glTexCoord2f(1.0f, 0.0f);glVertex3f(30.0f,  1.5f, -22.0f);
			glEnd();
			//�����
			glBegin(GL_QUADS);
			glNormal3f(0.0f, 0.0f, 1.0f); //���ڶ��巨������
			glTexCoord2f(0.0f, 0.0f);glVertex3f(-30.0f, 0, -22.0f); 
			glTexCoord2f(0.0f, 1.0f);glVertex3f(-30.0f, 1.5f, -22.0f); 
			glTexCoord2f(1.0f, 1.0f);glVertex3f(-30.0f,  1.5f, -30.0f); 
			glTexCoord2f(1.0f, 0.0f);glVertex3f(-30.0f,  0, -30.0f);
			glEnd();
			//��ǰ��
			glBegin(GL_QUADS);
			glNormal3f(0.0f, 1.0f, 0.0f); //���ڶ��巨������
			glTexCoord2f(0.0f, 0.0f);glVertex3f(-30.0f, 0, -22.0f); 
			glTexCoord2f(0.0f, 1.0f);glVertex3f(-30.0f, 1.5f, -22.0f); 
			glTexCoord2f(1.0f, 1.0f);glVertex3f(30.0f,  1.5f, -22.0f); 
			glTexCoord2f(1.0f, 0.0f);glVertex3f(30.0f,  0, -22.0f);
			glEnd();
			//���ұ�
			glBegin(GL_QUADS);
			glNormal3f(0.0f, 1.0f, 0.0f); //���ڶ��巨������
			glTexCoord2f(0.0f, 0.0f);glVertex3f(30.0f, 0, -22.0f); 
			glTexCoord2f(0.0f, 1.0f);glVertex3f(30.0f, 1.5f, -22.0f); 
			glTexCoord2f(1.0f, 1.0f);glVertex3f(30.0f,  1.5f, -30.0f); 
			glTexCoord2f(1.0f, 0.0f);glVertex3f(30.0f,  0, -30.0f);
			glEnd();
			glDisable(GL_TEXTURE_2D);
			//���ƽ�̨
            //����̨����
			glBindTexture(GL_TEXTURE_2D, texsdesk);
			glEnable(GL_TEXTURE_2D);
			glBegin(GL_QUADS);
			glNormal3f(0.0f, 1.0f, 0.0f); //���ڶ��巨������
			glTexCoord2f(0.0f, 0.0f);glVertex3f(-15.0f, 1.5f, -24.0f);
			glTexCoord2f(0.0f, 1.0f);glVertex3f(-15.0f, 9.5f, -24.0f);
			glTexCoord2f(1.0f, 1.0f);glVertex3f(0.0f,  9.5f, -24.0f);
			glTexCoord2f(1.0f, 0.0f);glVertex3f(0.0f,  1.5f, -24.0f);
			glEnd();
			glBegin(GL_QUADS);
			glNormal3f(0.0f, 1.0f, 0.0f); //���ڶ��巨������
			glTexCoord2f(0.0f, 0.0f);glVertex3f(0.0f, 1.5f, -24.0f);
			glTexCoord2f(0.0f, 1.0f);glVertex3f(0.0f, 9.5f, -24.0f);
			glTexCoord2f(1.0f, 1.0f);glVertex3f(0.0f,  9.5f, -28.0f);
			glTexCoord2f(1.0f, 0.0f);glVertex3f(0.0f,  1.5f, -28.0f);
			glEnd();
			glBegin(GL_QUADS);
			glNormal3f(0.0f, 1.0f, 0.0f); //���ڶ��巨������
			glTexCoord2f(0.0f, 0.0f);glVertex3f(-15.0f, 1.5f, -24.0f);
			glTexCoord2f(0.0f, 1.0f);glVertex3f(-15.0f, 9.5f, -24.0f);
			glTexCoord2f(1.0f, 1.0f);glVertex3f(-15.0f,  9.5f, -28.0f);
			glTexCoord2f(1.0f, 0.0f);glVertex3f(-15.0f,  1.5f, -28.0f);
			glEnd();
			glBegin(GL_QUADS);
			glNormal3f(0.0f, 1.0f, 0.0f); //���ڶ��巨������
			glTexCoord2f(0.0f, 0.0f);glVertex3f(-15.0f, 9.5f, -24.0f);
			glTexCoord2f(0.0f, 1.0f);glVertex3f(-15.0f, 9.5f, -26.0f);
			glTexCoord2f(1.0f, 1.0f);glVertex3f(0.0f,  9.5f, -26.0f);
			glTexCoord2f(1.0f, 0.0f);glVertex3f(0.0f,  9.5f, -24.0f);
			glEnd();
			//����
            glColor3f(0.521f,0.121f,0.0547f);
            glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texdoor);
			glBegin(GL_QUADS);
			glNormal3f(-1.0f, 0.0f, 0.0f); //���ڶ��巨������
			glTexCoord2f(0.0f, 0.0f);glVertex3f(39.9f, 0.0f, -25.0f); 
			glTexCoord2f(0.0f, 1.0f);glVertex3f(39.9f, 14.0f, -25.0f); 
			glTexCoord2f(1.0f, 1.0f);glVertex3f(39.9f, 14.0f, -19.0f); 
			glTexCoord2f(1.0f, 0.0f);glVertex3f(39.9f,  0.0f, -19.0f);
			glEnd();
			glDisable(GL_TEXTURE_2D);

}
void drawdesks()
{
//������
    GLfloat desk[]={0.6, 0.3, 0.1};
    for(int y=0;y<=4;y++)
    {
        for(int x=-1;x<=1;x++)
        {
//�����ϱ�
            glColor4f(0.6, 0.3, 0.1, 1);
            glPushMatrix();
            glTranslatef(x*20,5.1,-17.5+y*8);
            glScalef(10,0.2,3);
            glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,desk);
            glutSolidCube(1.0f);
            glPopMatrix();
//����ǰ��
            glColor4f(0.6, 0.3, 0.1, 1);
            glPushMatrix();
            glTranslatef(x*20,0,-18.9+y*8);
            glScalef(10,10,0.2);
            glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,desk);
            glutSolidCube(1.0f);
            glPopMatrix();
        }
    }
}
void drawchairs()
{
    GLfloat chair[] = {0.6, 0.3, 0.1}; // ��ɫ
    float chairWidth = 10.0f; // ���ӿ��
    float chairDepth = 3.0f; // �������
    float legHeight = 6.0f; // �����ȸ߶�
    int numChairs = 3; // ��������
    float totalWidth = numChairs * chairWidth + (numChairs - 1) * 10; // �������ӵ��ܿ��
    float initialPosition = -totalWidth / 2.0f + chairWidth / 2.0f; // �����ʼλ��

    for(int j = 0; j <= 4; j++)
    {
        for(int i = 0; i < numChairs; i++) // ѭ����������
        {
            // �����ӵײ�
            glColor3f(0.6, 0.3, 0.1); // ��ɫ
            glPushMatrix();
            glTranslatef(initialPosition + i * (chairWidth + 10), 3.1, -14.5 + j * 8); // ���ݼ���õ���λ�ðڷ�����
            glScalef(chairWidth, 0.2, chairDepth);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, chair);
            glutSolidCube(1.0f);
            glPopMatrix();

            // ��������
            glColor3f(0.3, 0.15, 0.05); // ����ɫ
            glPushMatrix();
            glTranslatef(initialPosition + i * (chairWidth + 10) - 5, 0, -13 + j * 8); // ���ݼ���õ���λ�ðڷ�������
            glScalef(0.2, legHeight, 0.2);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, chair);
            glutSolidCube(1.0f);
            glPopMatrix();
            glPushMatrix();
            glTranslatef(initialPosition + i * (chairWidth + 10) + 5, 0, -13 + j * 8); // ���ݼ���õ���λ�ðڷ�������
            glScalef(0.2, legHeight, 0.2);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, chair);
            glutSolidCube(1.0f);
            glPopMatrix();
        }
    }
}


void reshape(int we,int he)
{
	WinWidth=we;
	WinHeight=he;
	glViewport(0,0,(GLsizei) we, (GLsizei) he);
    glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	gluPerspective(90.0f, (GLfloat)we/(GLfloat)he, 0.01f,100.0f);
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
	gluLookAt(myEye.x, myEye.y, myEye.z, vPoint.x+30*sin(vAngle), vPoint.y,-30*cos(vAngle), 0.0f, 1.0f, 0.0f);
}
void myDisplay()
{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        drawscence();
		drawdesks();
		drawchairs();
		glFlush();
}

GLvoid OnKeyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 97:
		myEye.x-=0.5;
		vPoint.x-=0.5;
		if(myEye.x<=-40)
			myEye.x=-40;
		break;
	case 100:
		myEye.x+=0.5;
		vPoint.x+=0.5;
        if(myEye.x>=40)
			myEye.x=40;
		break;
	case 119:
        myEye.z-=0.5;
		 if(myEye.z<=-30)
			 myEye.z=-30;
		 break;
	case 115:
         myEye.z+=0.5;
		 if(myEye.z>=30)
			 myEye.z=30;
		 break;
	case 27:
		exit(0);

	}
	reshape(WinWidth,WinHeight);
	glutPostRedisplay();

}
GLvoid OnSpecial(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_LEFT:
		 vAngle-=0.05;
		 break;
	case GLUT_KEY_RIGHT:
		 vAngle+=0.05;
		 break;
	case GLUT_KEY_UP:
		 myEye.y+=0.05;
         if(myEye.y>=30)
			 myEye.y=30;
		 break;
	case GLUT_KEY_DOWN:
		 myEye.y-=0.5;
		 if(myEye.y<=0)
			 myEye.y=30;
		 break;
	case GLUT_KEY_PAGE_DOWN:
		 myEye.z+=0.5;
		 if(myEye.z>=30)
			 myEye.z=30;
		 break;
	case GLUT_KEY_PAGE_UP:
		 myEye.z-=0.5;
		 if(myEye.z<=-30)
			 myEye.z=-30;
		 break;
	case GLUT_KEY_F2:
		 glEnable(GL_LIGHT1);
		 break;
	case GLUT_KEY_F1:
		 glDisable(GL_LIGHT1);
}
	reshape(WinWidth,WinHeight);
	glutPostRedisplay();
}
GLvoid OnIdle()
{
	glutPostRedisplay();
}

void initial()
{
    glClearColor(0,0,0,0);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,model_ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);

	glLightfv(GL_LIGHT0,GL_POSITION,light_position0);
	glLightfv(GL_LIGHT0,GL_AMBIENT,mat_ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,light);
	glLightfv(GL_LIGHT0,GL_SPECULAR,light);

	
	glLightfv(GL_LIGHT1,GL_POSITION,light_position1);
	glLightfv(GL_LIGHT1,GL_AMBIENT,mat_ambient);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,white_light);
	glLightfv(GL_LIGHT1,GL_SPECULAR,white_light);


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);	
	glEnable(GL_COLOR_MATERIAL);	


	glShadeModel(GL_SMOOTH);				
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);	//ָ��������ɫ����
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);	//ָ�����϶Ծ����ķ���
	glEnable(GL_DEPTH_TEST);//	
}

int main(int argc, char* argv[]) 
{ 
	myEye.x=0;
	myEye.y=15;
	myEye.z=25;
    vPoint.x=0;
	vPoint.y=15;
	vPoint.z=-30;
    vAngle=0;
    glEnable(GL_DEPTH_TEST);
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE); 
	glutInitWindowPosition(400, 0); 
	glutInitWindowSize(800, 600); 
	glutCreateWindow("classroom"); 
	initial();
	glutDisplayFunc(&myDisplay);
	glutReshapeFunc(reshape);
    glutKeyboardFunc(OnKeyboard);
	glutSpecialFunc(OnSpecial);
    glutIdleFunc(OnIdle);

    texblackboard=load_texture("blackboard.bmp");
	texwindow=load_texture("window.bmp");
    picture= load_texture("picture.bmp");
	texceiling=load_texture("ceiling.bmp");
	texdoor=load_texture("door.bmp");
	texbackwall=load_texture("backwall.bmp");
	texgaodi=load_texture("gaodi.bmp");
	texsdesk=load_texture("sdesk.bmp");
	texclock=load_texture("clock.bmp");

	glutMainLoop();
    return 0; 
 }
//	�ϼ����¼��ֱ�����ӽ����º�����
//	������Ҽ��ֱ���������Ӻ����һ���
//	w,s,a,d���ֱ��ʾ��ǰ�������ң�����ƽ��
//	pgup��pgdn�ֱ������ǰ���κ��������
//	"F1��F2���ֱ���ƿ��ơ��ص�