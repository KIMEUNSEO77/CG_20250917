#include <iostream>
#include <glew.h>
#include <freeglut.h>
#include <freeglut_ext.h>
#include "Rect.h"
#include "Eraser.h"

int width = 800;
int height = 600;

int removedCount = 0; // ���ŵ� �簢���� ����

float currentScale = 1.0f;  // ���� ���찳 ũ��

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

// �浹 üũ�� �غ�!!!!
void GetCollid(const Rec& r, float& left, float& right, float& bottom, float& top)
{
    float halfW = (r.width * r.scale) * 0.5f;
    float halfH = (r.height * r.scale) * 0.5f;
    left = r.posX - halfW;
    right = r.posX + halfW;
    bottom = r.posY - halfH;
    top = r.posY + halfH;
}

// �浹 üũ
bool CheckCollid(float l1, float r1, float b1, float t1,
    float l2, float r2, float b2, float t2)
{
    if (r1 < l2 || r2 < l1) return false;
    if (t1 < b2 || t2 < b1) return false;

    return true;
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
			eraser.scale = currentScale;
            glutPostRedisplay(); // ȭ�� ����
        }
        else if (state == GLUT_UP)
        {
			eraseMode = false;
        }
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        if (removedCount > 0)
        {
            Rec temp{};
			temp.posX = nx;
			temp.posY = ny;
            RandomColor(temp.color);
            temp.scale = 1.0f;
            temp.width = recW;
            temp.height = recH;
            rects.push_back(temp);
			removedCount--;
            currentScale -= 0.1f;
        }
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

    float l1, r1, b1, t1;
    GetCollid(eraser, l1, r1, b1, t1);

    for (int i = rects.size() - 1; i >= 0; --i)
    {
        float l2, r2, b2, t2;
        GetCollid(rects[i], l2, r2, b2, t2);
        if (CheckCollid(l1, r1, b1, t1, l2, r2, b2, t2))
        {
			eraser.color[0] = rects[i].color[0];
			eraser.color[1] = rects[i].color[1];
            eraser.color[2] = rects[i].color[2];
            if (rectMove)
            {
                eraser.scale += 0.2f;
				currentScale = eraser.scale;
            }
            else 
            {
                eraser.scale += 0.1f; 
                currentScale = eraser.scale;
            }
            rects.erase(rects.begin() + i);
			removedCount++;
        }
	}

    MouseInside(eraser);

    glutPostRedisplay();
}

// ����
void Reset()
{
    rects.clear();
    InitRects();
	InitEraser();
	eraser.scale = 1.0f;
    currentScale = eraser.scale;
	removedCount = 0;
	rectMove = false;
}

void Timer(int value)
{
    if (rectMove)
    {
        for (int i = 0; i < rects.size(); ++i)
        {
            MoveRect(rects[i], 0.01f, 0.01f);
            MouseInside(rects[i]);
        }
        glutPostRedisplay();  // ȭ�� ����
    }
	glutTimerFunc(50, Timer, 0);   // Ÿ�̸� �ݹ� �Լ� ����
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        // �簢�� ũ�� Ű���
        case '+':
            for (int i = 0; i < rects.size(); ++i)
            {
                rects[i].scale += 0.1f;
                MouseInside(rects[i]);
            }
            glutPostRedisplay();  // ȭ�� ����
	        break;
		// �簢�� ũ�� ���̱�
        case '-':
            for (int i = 0; i < rects.size(); ++i)
            {
                rects[i].scale -= 0.1f;
                if (rects[i].scale < 0.3f) rects[i].scale = 0.1f;
                MouseInside(rects[i]);
            }
            glutPostRedisplay();  // ȭ�� ����
			break;
            // �簢�� �����̰� �ϱ�
        case 'm':
			rectMove = !rectMove;
            break;
        case 'r':
            Reset();
            glutPostRedisplay();  // ȭ�� ����
            break;
        case 'q':
            exit(0);
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
    glutTimerFunc(50, Timer, 0);   // Ÿ�̸� �ݹ� �Լ�

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