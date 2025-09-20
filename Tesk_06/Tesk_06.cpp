#include <iostream>
#include <glew.h>
#include <freeglut.h>
#include <freeglut_ext.h>
#include <vector>
#include <random>
#include "Rec.h"
using namespace std;

int width = 800;
int height = 600;

vector<Rec> rects;  // �簢����

int currentMove = 1;
bool gRunning = false;      // �����̴� ������
bool gTimerActive = false;  // Ÿ�̸� ���ۿ���
float gSpeed = 0.005f;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

// 5~10���� �簢���� ���� ��ġ, �������� ����
void InitRects()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(5, 10);
    int rectCount = dis(gen);

    rects.clear();
    rects.reserve(rectCount);
    for (int i = 0; i < rectCount; ++i) 
    {
        rects.emplace_back(); // push_back�̶� �ݴ�� �ٷ� ��ü ����
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

// ��� �簢�� Ŭ���ϴ���
int PickRect(float nx, float ny) 
{
    for (int i = (int)rects.size() - 1; i >= 0; --i)
    {
        if (rects[i].HitRect(nx, ny)) return i;
    }
    return -1;
}

// �簢���� 4���
// �θ� �߽�(cx,cy)���� ȭ����� ��и� ��ġ�� �̵����� �ִ� �Լ�
void PositionQuads(std::array<Rec, 4>& kids, const Rec& parent) 
{
    const float cx = parent.getPosX();
    const float cy = parent.getPosY();
    const float dx = (parent.getWidth() * parent.getScale()) * 0.25f; // ȭ��� ���� ������ ����
    const float dy = (parent.getHeight() * parent.getScale()) * 0.25f; // ȭ��� ���� ������ ����

	float offset = 0.01f; // �ణ�� ���� ����

    // ����, ����, �»�, ��� (������ �������)
    kids[0].SetPos(cx - dx - offset, cy - dy - offset); // LB
    kids[1].SetPos(cx + dx + offset, cy - dy - offset); // RB
    kids[2].SetPos(cx - dx - offset, cy + dy + offset); // LT
    kids[3].SetPos(cx + dx + offset, cy + dy + offset); // RT
}

// ȭ�� ��迡 �����ϸ� ���������
void CheckBounds(Rec& r)
{
    float halfW = 0.5f * r.getWidth() * r.getScale();
    float halfH = 0.5f * r.getHeight() * r.getScale();
    float left = r.getPosX() - halfW;
    float right = r.getPosX() + halfW;
    float bottom = r.getPosY() - halfH;
    float top = r.getPosY() + halfH;
    if (right < -1.0f || left > 1.0f || top < -1.0f || bottom > 1.0f)
    {
        r.SetPos(100.0f, 100.0f); // ȭ�� ������ ������
    }
}
void StepAll(std::vector<Rec>& rects) 
{
    for (auto& r : rects) 
    {
        // ��ġ ����
        r.SetPos(r.getPosX() + r.vx, r.getPosY() + r.vy);
        CheckBounds(r);
    }
}

// Ÿ�̸� �ݹ�
void Timer(int)
{
    if (!gRunning) 
    {         // ���� ���¸� Ÿ�̸� ü�� ����
        gTimerActive = false;
        return;
    }

    StepAll(rects);        
    glutPostRedisplay();

    glutTimerFunc(16, Timer, 0);
}

// �� �������°� �����ϱ� ���� Ÿ�̸Ӱ� �� ���� ����ǵ���
void EnsureTimer()
{
    if (!gTimerActive)
    {
        gTimerActive = true;
        glutTimerFunc(16, Timer, 0); // ù ����
    }
}

void Animation(int hit)
{
    if (hit < 0 || hit >= (int)rects.size()) return;

    auto kids = rects[hit].MakeChild();

    PositionQuads(kids, rects[hit]); 

    // �θ� ���� & 4�� ����
    rects.erase(rects.begin() + hit);
    int base = (int)rects.size();
    rects.insert(rects.end(), kids.begin(), kids.end());

    // �¿����
    if (currentMove == 1)
    {
        rects[base + 0].vy = -gSpeed;  
        rects[base + 1].vx = gSpeed; 
        rects[base + 2].vx = -gSpeed;
        rects[base + 3].vy = gSpeed;
    }
    // �밢��
    else if (currentMove == 2)
    {
        // �밢�� �ӵ� �ο�: �»�/���/����/����
        rects[base + 0].vx = -gSpeed; rects[base + 0].vy = -gSpeed; // left-down
        rects[base + 1].vx = gSpeed; rects[base + 1].vy = -gSpeed; // right-down
        rects[base + 2].vx = -gSpeed; rects[base + 2].vy = gSpeed; // left-up
        rects[base + 3].vx = gSpeed; rects[base + 3].vy = gSpeed; // right-up
	}
    // �������θ�
    else if (currentMove == 3)
    {
        int dir = rand() % 2;
        // ����
        if (dir == 0)
        {
            rects[base + 0].vx = -gSpeed;
            rects[base + 1].vx = -gSpeed;
            rects[base + 2].vx = -gSpeed;
            rects[base + 3].vx = -gSpeed;
        }
        // ������
        else if (dir == 1)
        {
            rects[base + 0].vx = gSpeed;
            rects[base + 1].vx = gSpeed;
            rects[base + 2].vx = gSpeed;
            rects[base + 3].vx = gSpeed;
        }
    }
    gRunning = true; // ������Ʈ ����
    EnsureTimer();
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
			int hit = PickRect(nx, ny);
            if (hit != -1) // �簢���� Ŭ���ߴٸ�
            {
				Animation(hit);

                glutPostRedisplay();
            }
        }
    }
}

// Ű���� �ݹ�
GLvoid Keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case '1':
		currentMove = 1;
        break;
    case '2':
		currentMove = 2;
        break;
    case '3':
        currentMove = 3;
        break;
    case '4':
		currentMove = 4;
        break;
    }
}

void main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(width, height);
    glutCreateWindow("Tesk_06");

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
	glutKeyboardFunc(Keyboard);
    
    glutMainLoop();
}

GLvoid drawScene()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (const auto& r : rects) r.draw();

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}