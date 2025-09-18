#include <iostream>
#include <glew.h>
#include <freeglut.h>
#include <freeglut_ext.h>
#include <vector>
#include <random>
using namespace std;

int width = 800;
int height = 600;

float recW = 0.06f;  // �簢���� �ʺ�
float recH = 0.08f;  // �簢���� ����

struct Rec
{
    float color[3];
    float scale;
    float posX, posY;
    float width, height;
};
vector<Rec> rects;  // �簢����
Rec eraser;         // ���찳

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

// ���� ���� ����
void RandomColor(float color[3])
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.01f, 1.0f);
    color[0] = dis(gen);
    color[1] = dis(gen);
    color[2] = dis(gen);
}

// ���� ��ġ ����
void RandomPosition(float& posX, float& posY)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    posX = dis(gen);
    posY = dis(gen);
}

// �簢�� �׸���
void DrawRect(const Rec& rect)
{
    glPushMatrix();
    glTranslatef(rect.posX, rect.posY, 0.0f);  // �簢���� �߽��� posX, posY�� �̵�
    glScalef(rect.scale, rect.scale, 1.0f);
    glColor3fv(rect.color);
    glRectf(
        -rect.width / 2, -rect.height / 2,
        rect.width / 2, rect.height / 2
    );
    glPopMatrix();
}

// 20~40���� �簢���� ���� ��ġ, �������� ����
void InitRects()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(20, 40);
    int rectCount = dis(gen);
    for (int i = 0; i < rectCount; ++i)
    {
        Rec temp{};
        RandomPosition(temp.posX, temp.posY);
        RandomColor(temp.color);
        temp.scale = 1.0f;
        temp.width = recW;
        temp.height = recH;
        rects.push_back(temp);
    }
}

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

// ���찳 ����
void InitEraser()
{
    eraser.color[0] = 0.0f;
    eraser.color[1] = 0.0f;
    eraser.color[2] = 0.0f;
    eraser.scale = 1.0f;
    eraser.width = recW * 2.0f;
    eraser.height = recH * 2.0f;
}

// ���찳 �׸���
void DrawEraser()
{
    glPushMatrix();
    glTranslatef(eraser.posX, eraser.posY, 0.0f);  // �簢���� �߽��� posX, posY�� �̵�
    glScalef(eraser.scale, eraser.scale, 1.0f);
    glColor3fv(eraser.color);
    glRectf(
        -eraser.width / 2, -eraser.height / 2,
        eraser.width / 2, eraser.height / 2
    );
    glPopMatrix();
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
			eraser.posX = nx;
			eraser.posY = ny;
			MouseInside(eraser);
			InitEraser();
            glutPostRedisplay(); // ȭ�� ����
        }
        else if (state == GLUT_UP)
        {
            
        }
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
       
    }
    glutPostRedisplay();
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

    glutMouseFunc(Mouse);

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
    DrawEraser();

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}