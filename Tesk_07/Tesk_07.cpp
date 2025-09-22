#include <iostream>
#include <glew.h>
#include <freeglut.h>
#include <freeglut_ext.h>
#include <vector>
#include "Rec.h"
#include <random>
using namespace std;

const int rectCount = 10;

int width = 800;
int height = 600;

vector<Rec> rects(rectCount);        // 사각형들
vector<Rec> rectLines(rectCount);    // 보드판

int dragIndex = -1;                         // 드래그 중인 사각형
float dragMouseX = 0.0f, dragMouseY = 0.0f; // 드래그 중인 사각형과 마우스의 상대좌표    

float gSpeed = 0.02f;
int movingIdx = -1; // 움직이는 사각형 인덱스
bool moving = false;
bool timerActive = false;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

void InitRects()
{
    for (int i = 0; i < rectCount; ++i)
    {
        InitRec(rects[i]);
    }
}

// 충돌 체크할 준비!!!!
void GetCollid(const Rec& r, float& left, float& right, float& bottom, float& top)
{
    float halfW = (r.width * r.scale) * 0.5f;
    float halfH = (r.height * r.scale) * 0.5f;
    left = r.posX - halfW;
    right = r.posX + halfW;
    bottom = r.posY - halfH;
    top = r.posY + halfH;
}

// 충돌 체크
bool CheckCollid(float l1, float r1, float b1, float t1,
    float l2, float r2, float b2, float t2)
{
    if (r1 < l2 || r2 < l1) return false;
    if (t1 < b2 || t2 < b1) return false;

    return true;
}

// 사각형들의 보드판을 라인으로 그리기
void InitLineBoard()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> disX(-0.9f, -0.1f);
    std::uniform_real_distribution<float> disY(-0.9f, 0.9f);

    for (int i = 0; i < rectCount; ++i)
    {
        rectLines[i] = rects[i];
        rectLines[i].scale = 1.0f;

        const int maxTry = 500;
        bool placed = false;

        for (int tries = 0; tries < maxTry && !placed; ++tries)
        {
            // 좌표 뽑기
            rectLines[i].posX = disX(gen);
            rectLines[i].posY = disY(gen);

            float l1, r1, b1, t1;
            GetCollid(rectLines[i], l1, r1, b1, t1);

            // 배치된 보드판들과 충돌 검사
            placed = true;
            for (int j = 0; j < i; ++j)
            {
                float l2, r2, b2, t2;
                GetCollid(rectLines[j], l2, r2, b2, t2);
                if (CheckCollid(l1, r1, b1, t1, l2, r2, b2, t2))
                {
                    placed = false; // 다시 뽑기
                    break;
                }
            }
        }
	}
}

// 사각형 보드판을 서로 다 붙혀서 그리기
void InitLineBoard2()
{
    float nextX = -0.8f;
    float nextY = 0.5f;

    for (int i = 0; i < rectCount; i++)
    {
        rectLines[i] = rects[i];
        rectLines[i].scale = 1.0f;
    }
  
    for (int i = 0; i < rectCount/2; ++i)
    {
        rectLines[i].posX = nextX;
        rectLines[i].posY = nextY;
        nextX = rectLines[i].posX + (rectLines[i].width * rectLines[i].scale * 0.5f + 
            rectLines[i+1].width * rectLines[i+1].scale * 0.5f);
	}

	nextX = -0.8f;
    nextY = rectLines[0].posY - (rectLines[rectCount -1].height * rectLines[rectCount - 1].scale * 0.5f +
        rectLines[0].height * rectLines[0].scale * 0.5f);

    int index = 1;
	float offset = 0.1f;

    for (int i = rectCount - 1; i >= rectCount / 2; i--)
    {
        rectLines[i].posX = nextX;
        rectLines[i].posY = nextY;
        nextX = rectLines[i].posX + (rectLines[i].width * rectLines[i].scale * 0.5f +
			rectLines[i - 1].width * rectLines[i - 1].scale * 0.5f);
        nextY = rectLines[index].posY - (rectLines[i - 1].height * rectLines[i - 1].scale * 0.5f +
			rectLines[index].height * rectLines[index].scale * 0.5f) - offset;

        index++;
        offset += 0.05f;
    }
}

// 사각형 보드판을 나눠서 그리기
void InitLineBoard3()
{
    float nextX = -0.8f;
    float nextY = 0.7f;

    for (int i = 0; i < rectCount; i++)
    {
        rectLines[i] = rects[i];
        rectLines[i].scale = 1.0f;
    }
    
    for (int i = 0; i < 5; i++)
    {
		rectLines[i].posX = nextX;
        rectLines[i].posY = nextY;
        nextY = rectLines[i].posY - (rectLines[i].height * rectLines[i].scale * 0.5f +
			rectLines[i + 1].height * rectLines[i + 1].scale * 0.5f);
    }

    nextX = rectLines[4].posX + (rectLines[4].width * rectLines[4].scale * 0.5f +
        rectLines[5].width * rectLines[5].scale * 0.5f);
	nextY = rectLines[4].posY - 0.05f;

    for (int i = 5; i < 8; i++)
    {
        rectLines[i].posX = nextX;
        rectLines[i].posY = nextY;
        nextX = rectLines[i].posX + (rectLines[i].width * rectLines[i].scale * 0.5f +
            rectLines[i + 1].width * rectLines[i + 1].scale * 0.5f);
    }

	rectLines[8].posX = rectLines[7].posX + 0.05f;
	rectLines[8].posY = rectLines[7].posY + (rectLines[7].height * rectLines[7].scale * 0.5f +
        rectLines[8].height * rectLines[8].scale * 0.5f);

	rectLines[9].posX = rectLines[8].posX + 0.05f;
    rectLines[9].posY = rectLines[8].posY + (rectLines[8].height * rectLines[8].scale * 0.5f +
		rectLines[9].height * rectLines[9].scale * 0.5f);
}

void DrawLineBoard()
{
    for (int i = 0; i < rectCount; ++i)
    {
        // 선 모드로만 그리기
        glPushAttrib(GL_POLYGON_BIT | GL_LINE_BIT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(2.0f);
        DrawRect(rectLines[i]); // 폴리곤모드가 라인으로 바꿔줌
        glPopAttrib();
	}
}

// 마우스 좌표 화면 밖으로 못 나가게
void MouseInside(Rec& r)
{
    float w = (r.width * r.scale) * 0.5f;
    float h = (r.height * r.scale) * 0.5f;

    if (r.posX < -1.0f + w) r.posX = -1.0f + w;
    if (r.posX > 1.0f - w) r.posX = 1.0f - w;
    if (r.posY < -1.0f + h) r.posY = -1.0f + h;
    if (r.posY > 1.0f - h) r.posY = 1.0f - h;
}

// 마우스 클릭한 좌표 정규화
void PixelTrans(int px, int py, float& nx, float& ny)
{
    float w = (float)glutGet(GLUT_WINDOW_WIDTH);
    float h = (float)glutGet(GLUT_WINDOW_HEIGHT);
    nx = (px / w) * 2.0f - 1.0f;
    ny = 1.0f - (py / h) * 2.0f;
}

bool HitRect(Rec& rect, float nx, float ny)
{
    float halfW = 0.5f * rect.width * rect.scale;
    float halfH = 0.5f * rect.height * rect.scale;

    float left = rect.posX - halfW;
    float right = rect.posX + halfW;
    float bottom = rect.posY - halfH;
    float top = rect.posY + halfH;

    return (nx >= left && nx <= right && ny >= bottom && ny <= top);
}

// 어느 사각형 클릭하는지
int PickRect(float nx, float ny)
{
    for (int i = (int)rects.size() - 1; i >= 0; --i)
    {
        if (HitRect(rects[i],nx, ny)) return i;
    }
    return -1;
}

// 드래그 종료했을 때 보드판에 맞는지 체크
bool checkSuit(int index)
{
    if (index < 0 || index >= (int)rects.size() || index >= (int)rectLines.size())
        return false;

    const float offset = 0.03f;        // 오차범위
    const float offset2 = offset * offset;

    float dx = rects[index].posX - rectLines[index].posX;
    float dy = rects[index].posY - rectLines[index].posY;

    return (dx * dx + dy * dy) < offset2;
}

// 마우스 콜백
void Mouse(int button, int state, int x, int y)
{
    float nx, ny;
    PixelTrans(x, y, nx, ny);

    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            int hit = PickRect(nx, ny);
            if (hit != -1)
            {
				dragIndex = hit;
                dragMouseX = rects[hit].posX - nx;
                dragMouseY = rects[hit].posY - ny;
                glutPostRedisplay();
            }
        }
        else if (state == GLUT_UP)
        {
            if (checkSuit(dragIndex))
            {
                rects[dragIndex].posX = rectLines[dragIndex].posX;
                rects[dragIndex].posY = rectLines[dragIndex].posY;

                printf("맞았음\n");
                fflush(stdout);
            }
            dragIndex = -1; // 드래그 종료
        }
    }
}

// 마우스 드래그 콜백
void MouseDrag(int x, int y)
{
    float nx, ny;
    PixelTrans(x, y, nx, ny);

    // 마우스 커서가 중심으로 바로 가는걸 방지
    rects[dragIndex].posX = nx + dragMouseX;
    rects[dragIndex].posY = ny + dragMouseY;

    MouseInside(rects[dragIndex]);

    glutPostRedisplay();
}

void Reset()
{
    InitRects();
    InitLineBoard();
	dragIndex = -1;
    float dragMouseX = 0.0f, dragMouseY = 0.0f;
    glutPostRedisplay();
}

// 사각형을 보드판으로
void MoveRect(int idx, float speed)
{
    float dx = rectLines[idx].posX - rects[idx].posX;
    float dy = rectLines[idx].posY - rects[idx].posY;
	float dist = sqrtf(dx * dx + dy * dy);   // 사각형과 보드판 사이 거리

    if (dist <= speed)
    {
        rects[idx].posX = rectLines[idx].posX;
        rects[idx].posY = rectLines[idx].posY;
        movingIdx = (movingIdx + 1) % rectCount;
		return;
    }

    // 한 스텝 전진
    rects[idx].posX += dx / dist * speed;
    rects[idx].posY += dy / dist * speed;
}

// 타이머
void Timer(int value)
{
    if (moving)
    {
        if (movingIdx == -1)
			movingIdx = 0;
        else if (movingIdx != -1)
        {
			MoveRect(movingIdx, gSpeed);
		}
    }

    // 타이머 재설정
    glutTimerFunc(50, Timer, 0);
    glutPostRedisplay();
}

// 키보드 콜백
GLvoid Keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'm':
		moving = !moving;

        if (!timerActive)
        {
            timerActive = true;
            glutTimerFunc(50, Timer, 0);
		}
        break;
    case 'r':
        Reset();
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
    glutInitWindowSize(width, height);
    glutCreateWindow("Tesk_07");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Unable to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "GLEW Initialized\n";

	InitRects();
	InitLineBoard3();    
	//InitLineBoard2();

    glutMouseFunc(Mouse);
    glutKeyboardFunc(Keyboard);
	glutMotionFunc(MouseDrag);

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutMainLoop();
}

GLvoid drawScene()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    for (const auto& r : rects) DrawRect(r);
    DrawLineBoard();

    glLoadIdentity();
    glLineWidth(2.0f);
    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_LINES);
    glVertex2f(0.2f, -1.0f);  // x=0 이면 화면 중앙 세로선
    glVertex2f(0.2f, 1.0f);
    glEnd();
    glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}