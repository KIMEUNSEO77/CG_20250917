#include "Rec.h"
#include <random>

namespace
{
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
        out[0] = randFloat(0.1f, 1.0f);
        out[1] = randFloat(0.1f, 1.0f);
        out[2] = randFloat(0.1f, 1.0f);
    }
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

// �簢�� �ʱ�ȭ
void InitRec(Rec& rect)
{
    rect.posX = randFloat(0.35f, 1.0f);
	rect.posY = randFloat(-1.0f, 0.8f);
    randomColor(rect.color);
    rect.scale = 1.0f;
    rect.width = randFloat(0.1f, 0.3f);
    rect.height = randFloat(0.1f, 0.5f);
}