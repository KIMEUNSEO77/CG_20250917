#include "Rect.h"
#include <random>
#include <glew.h>
#include <freeglut.h>

float recW = 0.06f;  // �簢���� �ʺ�
float recH = 0.08f;  // �簢���� ����
bool rectMove = false; // �簢�� �����̱� ���

std::vector<Rec> rects;  // �簢����

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

		temp.vx = 0.02f;
		temp.vy = 0.01f; 
        rects.push_back(temp);
    }
}

// �簢���� ���� �߰�
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

// ���� ��Ҵ��� Ȯ���ϴ� �Լ�
bool IsWall(const Rec& rec, float x, float y)
{
    float halfW = rec.width * rec.scale * 0.5f;
    float halfH = rec.height * rec.scale * 0.5f;

    // ����ȭ ��ǥ
    if (x - halfW <= -1.0f) return true; // ����
    if (x + halfW >= 1.0f) return true; // ������
    if (y - halfH <= -1.0f) return true; // �Ʒ�
    if (y + halfH >= 1.0f) return true; // ��
    return false;
}

// �簢�� �����̱�
void MoveRect(Rec& rect, float dx, float dy)
{
    if (IsWall(rect, rect.posX + rect.vx, rect.posY)) 
        rect.vx = -rect.vx; 

    if (IsWall(rect, rect.posX, rect.posY + rect.vy)) 
        rect.vy = -rect.vy;
    

    rect.posX += rect.vx;
    rect.posY += rect.vy;
}