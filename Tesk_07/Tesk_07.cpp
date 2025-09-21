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

// 왼쪽에 사각형들의 보드판을 라인으로 그리기
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

        const int MAX_TRIES = 500;
        bool placed = false;

        for (int tries = 0; tries < MAX_TRIES && !placed; ++tries)
        {
            // 새 좌표 뽑기
            rectLines[i].posX = disX(gen);
            rectLines[i].posY = disY(gen);

            float l1, r1, b1, t1;
            GetCollid(rectLines[i], l1, r1, b1, t1);

            // 이미 배치된 선 도형들과만 충돌 검사 (j < i)
            placed = true;
            for (int j = 0; j < i; ++j)
            {
                float l2, r2, b2, t2;
                GetCollid(rectLines[j], l2, r2, b2, t2);
                if (CheckCollid(l1, r1, b1, t1, l2, r2, b2, t2))
                {
                    placed = false; // 충돌 -> 다시 뽑기
                    break;
                }
            }
        }
	}
}

void DrawLineBoard()
{
    for (int i = 0; i < rectCount; ++i)
    {
        // 선 모드로만 그리기
        glPushAttrib(GL_POLYGON_BIT | GL_LINE_BIT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(2.0f);
        DrawRect(rectLines[i]); // 내부는 QUADS로 그려도 폴리곤모드가 라인으로 바꿔줌
        glPopAttrib();
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
	InitLineBoard();    

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
    glVertex2f(0.0f, -1.0f);  // x=0 이면 화면 중앙 세로선
    glVertex2f(0.0f, 1.0f);
    glEnd();
    glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}