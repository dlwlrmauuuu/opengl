//
// Created by 86155 on 2023/12/14.
//
// test00.cpp : Defines the entry point for the console application.
//

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include <GL/glut.h>
#include <GL/glaux.h>



int LoadGLTextures();

void myDisplay();
void myKeyboard(unsigned char key, int x, int y);
void myReshape(GLint w, GLint h);


GLuint	texture[3];			//  For 3 Textures

int main(int argc, char** argv)
{

    glutCreateWindow("纹理");


    glutDisplayFunc(myDisplay);
    glutKeyboardFunc(&myKeyboard);
    glutReshapeFunc(&myReshape);


    if (!LoadGLTextures())								// Jump To Texture Loading Routine
    {
        return FALSE;									// If Texture Didn't Load Return FALSE
    }

    glutMainLoop();
}


void myDisplay()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    glEnable(GL_TEXTURE_2D);	//允许使用纹理

    glBindTexture(GL_TEXTURE_2D, texture[2]);// 修改texture[0.。。2 可使用不同纹理。


    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-0.5, -0.5);

    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-0.5, 0.5);

    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(0.5, 0.5);

    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(0.5, -0.5);
    glEnd();


    glFlush();
}




void myKeyboard(unsigned char key, int x, int y)
{


    switch (key)
    {

        case'Q':
        case 'q':
            exit(0);
    }

    myDisplay();

}


void myReshape(GLint w, GLint h)
{

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



AUX_RGBImageRec *LoadBMP(char *Filename)				// Loads A Bitmap Image
{
    FILE *File=NULL;									// File Handle

    if (!Filename)										// Make Sure A Filename Was Given
    {
        return NULL;									// If Not Return NULL
    }

    File=fopen(Filename,"r");							// Check To See If The File Exists

    if (File)											// Does The File Exist?
    {
        fclose(File);									// Close The Handle
        return auxDIBImageLoad(Filename);				// Load The Bitmap And Return A Pointer
    }

    return NULL;										// If Load Failed Return NULL
}



int LoadGLTextures()									// Load Bitmaps And Convert To Textures
{
    int Status=FALSE;									// Status Indicator

    AUX_RGBImageRec *TextureImage[1];					// Create Storage Space For The Texture

    memset(TextureImage,0,sizeof(void *)*1);           	// Set The Pointer To NULL

    // Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
    TextureImage[0] = LoadBMP("D:\\opengl\\texture.bmp");
    if (TextureImage[0] != nullptr)
    {
        glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);

        Status=TRUE;									// Set The Status To TRUE

        glGenTextures(3, &texture[0]);					// Create Three Textures

        // Create Nearest Filtered Texture
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

        //	 Create Linear Filtered Texture
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

        // Create MipMapped Texture
        glBindTexture(GL_TEXTURE_2D, texture[2]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

    }

    if (TextureImage[0])								// If Texture Exists
    {
        if (TextureImage[0]->data)						// If Texture Image Exists
        {
            free(TextureImage[0]->data);				// Free The Texture Image Memory
        }

        free(TextureImage[0]);							// Free The Image Structure
    }

    return Status;										// Return The Status
}
