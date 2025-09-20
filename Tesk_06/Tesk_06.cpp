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

vector<Rec> rects;  // 사각형들

int currentMove = 1;
bool gRunning = false;      // 움직이는 중인지
bool gTimerActive = false;  // 타이머 동작여부
float gSpeed = 0.005f;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

// 5~10개의 사각형을 랜덤 위치, 색상으로 생성
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
        rects.emplace_back(); // push_back이랑 반대로 바로 객체 생성
    }
}

// 마우스 클릭한 좌표 정규화
void PixelTrans(int px, int py, float& nx, float& ny)
{
    float w = (float)glutGet(GLUT_WINDOW_WIDTH);
    float h = (float)glutGet(GLUT_WINDOW_HEIGHT);
    nx = (px / w) * 2.0f - 1.0f;
    ny = 1.0f - (py / h) * 2.0f;
}

// 어느 사각형 클릭하는지
int PickRect(float nx, float ny) 
{
    for (int i = (int)rects.size() - 1; i >= 0; --i)
    {
        if (rects[i].HitRect(nx, ny)) return i;
    }
    return -1;
}

// 사각형을 4등분
// 부모 중심(cx,cy)에서 화면상의 사분면 위치로 이동시켜 주는 함수
void PositionQuads(std::array<Rec, 4>& kids, const Rec& parent) 
{
    const float cx = parent.getPosX();
    const float cy = parent.getPosY();
    const float dx = (parent.getWidth() * parent.getScale()) * 0.25f; // 화면상 가로 절반의 절반
    const float dy = (parent.getHeight() * parent.getScale()) * 0.25f; // 화면상 세로 절반의 절반

	float offset = 0.01f; // 약간의 여유 공간

    // 좌하, 우하, 좌상, 우상 (순서는 마음대로)
    kids[0].SetPos(cx - dx - offset, cy - dy - offset); // LB
    kids[1].SetPos(cx + dx + offset, cy - dy - offset); // RB
    kids[2].SetPos(cx - dx - offset, cy + dy + offset); // LT
    kids[3].SetPos(cx + dx + offset, cy + dy + offset); // RT
}

// 화면 경계에 도달하면 사라지도록
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
        r.SetPos(100.0f, 100.0f); // 화면 밖으로 보내기
    }
}
void StepAll(std::vector<Rec>& rects) 
{
    for (auto& r : rects) 
    {
        // 위치 갱신
        r.SetPos(r.getPosX() + r.vx, r.getPosY() + r.vy);
        CheckBounds(r);
    }
}

// 타이머 콜백
void Timer(int)
{
    if (!gRunning) 
    {         // 멈춘 상태면 타이머 체인 종료
        gTimerActive = false;
        return;
    }

    StepAll(rects);        
    glutPostRedisplay();

    glutTimerFunc(16, Timer, 0);
}

// 더 빨라지는걸 방지하기 위해 타이머가 한 번만 예약되도록
void EnsureTimer()
{
    if (!gTimerActive)
    {
        gTimerActive = true;
        glutTimerFunc(16, Timer, 0); // 첫 예약
    }
}

void Animation(int hit)
{
    if (hit < 0 || hit >= (int)rects.size()) return;

    auto kids = rects[hit].MakeChild();

    PositionQuads(kids, rects[hit]); 

    // 부모 제거 & 4개 삽입
    rects.erase(rects.begin() + hit);
    int base = (int)rects.size();
    rects.insert(rects.end(), kids.begin(), kids.end());

    // 좌우상하
    if (currentMove == 1)
    {
        rects[base + 0].vy = -gSpeed;  
        rects[base + 1].vx = gSpeed; 
        rects[base + 2].vx = -gSpeed;
        rects[base + 3].vy = gSpeed;
    }
    // 대각선
    else if (currentMove == 2)
    {
        // 대각선 속도 부여: 좌상/우상/좌하/우하
        rects[base + 0].vx = -gSpeed; rects[base + 0].vy = -gSpeed; // left-down
        rects[base + 1].vx = gSpeed; rects[base + 1].vy = -gSpeed; // right-down
        rects[base + 2].vx = -gSpeed; rects[base + 2].vy = gSpeed; // left-up
        rects[base + 3].vx = gSpeed; rects[base + 3].vy = gSpeed; // right-up
	}
    // 한쪽으로만
    else if (currentMove == 3)
    {
        int dir = rand() % 2;
        // 왼쪽
        if (dir == 0)
        {
            rects[base + 0].vx = -gSpeed;
            rects[base + 1].vx = -gSpeed;
            rects[base + 2].vx = -gSpeed;
            rects[base + 3].vx = -gSpeed;
        }
        // 오른쪽
        else if (dir == 1)
        {
            rects[base + 0].vx = gSpeed;
            rects[base + 1].vx = gSpeed;
            rects[base + 2].vx = gSpeed;
            rects[base + 3].vx = gSpeed;
        }
    }
    gRunning = true; // 업데이트 시작
    EnsureTimer();
}

// 마우스 콜백
void Mouse(int button, int state, int x, int y)
{
    float nx, ny;  // 마우스가 클릭한 좌표를 정규화
    PixelTrans(x, y, nx, ny);

    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
			int hit = PickRect(nx, ny);
            if (hit != -1) // 사각형을 클릭했다면
            {
				Animation(hit);

                glutPostRedisplay();
            }
        }
    }
}

// 키보드 콜백
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