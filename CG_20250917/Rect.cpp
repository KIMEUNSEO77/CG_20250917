#include "Rect.h"
#include <random>
#include <glew.h>
#include <freeglut.h>

float recW = 0.06f;  // 사각형의 너비
float recH = 0.08f;  // 사각형의 높이

std::vector<Rec> rects;  // 사각형들

// 랜덤 색상 생성
void RandomColor(float color[3])
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.01f, 1.0f);
    color[0] = dis(gen);
    color[1] = dis(gen);
    color[2] = dis(gen);
}

// 랜덤 위치 생성
void RandomPosition(float& posX, float& posY)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    posX = dis(gen);
    posY = dis(gen);
}

// 사각형 그리기
void DrawRect(const Rec& rect)
{
    glPushMatrix();
    glTranslatef(rect.posX, rect.posY, 0.0f);  // 사각형의 중심을 posX, posY로 이동
    glScalef(rect.scale, rect.scale, 1.0f);
    glColor3fv(rect.color);
    glRectf(
        -rect.width / 2, -rect.height / 2,
        rect.width / 2, rect.height / 2
    );
    glPopMatrix();
}

// 20~40개의 사각형을 랜덤 위치, 색상으로 생성
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

// 사각형을 새로 추가
void AddRect()
{
    Rec temp{};
    RandomPosition(temp.posX, temp.posY);
    RandomColor(temp.color);
    temp.scale = 1.0f;
    temp.width = recW;
    temp.height = recH;
    rects.push_back(temp);
}