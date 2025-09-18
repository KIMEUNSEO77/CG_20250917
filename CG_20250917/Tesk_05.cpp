#include <iostream>
#include <glew.h>
#include <freeglut.h>
#include <freeglut_ext.h>
#include "Rect.h"
#include "Eraser.h"

int width = 800;
int height = 600;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

// ���콺 Ŭ���� ��ǥ ����ȭ
void PixelTrans(int px, int py, float& nx, float& ny)
{
    float w = (float)glutGet(GLUT_WINDOW_WIDTH);
    float h = (float)glutGet(GLUT_WINDOW_HEIGHT);
    nx = (px / w) * 2.0f - 1.0f;
    ny = 1.0f - (py / h) * 2.0f;
}

// �߽� ��ǥ ȭ�� ������ �� ������
void MouseInside(Rec& r)
{
    float w = (r.width * r.scale) * 0.5f;
    float h = (r.height * r.scale) * 0.5f;

    if (r.posX < -1.0f + w) r.posX = -1.0f + w;
    if (r.posX > 1.0f - w) r.posX = 1.0f - w;
    if (r.posY < -1.0f + h) r.posY = -1.0f + h;
    if (r.posY > 1.0f - h) r.posY = 1.0f - h;
}

// ���콺 �ݹ�
void Mouse(int button, int state, int x, int y)
{
    float nx, ny;  // ���콺�� Ŭ���� ��ǥ�� ����ȭ
    PixelTrans(x, y, nx, ny);

    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
			eraseMode = true;
			eraser.posX = nx;
			eraser.posY = ny;
			MouseInside(eraser);
			InitEraser();
            glutPostRedisplay(); // ȭ�� ����
        }
        else if (state == GLUT_UP)
        {
			eraseMode = false;
        }
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
       
    }
    glutPostRedisplay();
}

// ���콺 �巡�� �ݹ�
void MouseDrag(int x, int y)
{
    if (!eraseMode) return;

    float nx, ny;   // x, y�� ����ȭ�� ��ǥ
    PixelTrans(x, y, nx, ny);

    eraser.posX = nx;
    eraser.posY = ny;

    MouseInside(eraser);

    glutPostRedisplay();
}

// ����
void Reset()
{
    rects.clear();
    InitRects();
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'r':
        Reset();
        glutPostRedisplay();  // ȭ�� ����
        break;
    }
}

void main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(width, 600);
    glutCreateWindow("Tesk_05");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Unable to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "GLEW Initialized\n";

    InitRects();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);

    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse);
    glutMotionFunc(MouseDrag);

    glutMainLoop();
}

GLvoid drawScene()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < rects.size(); ++i)
    {
        DrawRect(rects[i]);
    }
	if (eraseMode) DrawEraser();

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}