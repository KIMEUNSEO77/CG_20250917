#include "Rec.h"
#include <random>

namespace 
{ // 이 파일 안에서만 쓰는 헬퍼들
    std::mt19937& rng() 
    {
        static std::mt19937 gen{ std::random_device{}() };
        return gen;
    }
    float randFloat(float a, float b) 
    {
        std::uniform_real_distribution<float> dis(a, b);
        return dis(rng());
    }
    void randomColor(float out[3]) 
    {
        out[0] = randFloat(0.01f, 1.0f);
        out[1] = randFloat(0.01f, 1.0f);
        out[2] = randFloat(0.01f, 1.0f);
    }
}

Rec::Rec() : scale(1.0f) 
{
    randomColor(color);
    posX = randFloat(-1.0f, 1.0f);
    posY = randFloat(-1.0f, 1.0f);
    width = randFloat(0.1f, 0.3f);
    height = randFloat(0.1f, 0.3f);
}

void Rec::draw() const 
{
    glPushMatrix();
    glTranslatef(posX, posY, 0.0f);
    glScalef(scale, scale, 1.0f);
    glColor3fv(color);
    glRectf(-width * 0.5f, -height * 0.5f,
        width * 0.5f, height * 0.5f);
    glPopMatrix();
}