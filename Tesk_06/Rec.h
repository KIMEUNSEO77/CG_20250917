#pragma once
#include <glew.h>
#include <freeglut.h>
#include <freeglut_ext.h>
#include <array>

class Rec 
{
public:
    Rec();              // ���� �ʱ�ȭ
    void draw() const;
    // �� �簢���� Ŭ���Ǿ�����(const: Ư�� ��� �Լ��� ��ü ���¸� �ٲ��� ������ ����)
	bool HitRect(float nx, float ny) const;
	std::array<Rec, 4> MakeChild() const; // 4���
	std::array<Rec, 8> MakeDoubleChild() const; // 8���

	float getPosX() const { return posX; }
	float getPosY() const { return posY; }
	float getWidth() const { return width; }
	float getHeight() const { return height; }
	float getScale() const { return scale; }

    // �ִϸ��̼ǿ� ����
    float vx = 0.0f, vy = 0.0f; // �ӵ�
    int groupId = -1;

    // ��ġ ���ſ�
    void SetPos(float x, float y) { posX = x; posY = y; }

private:
    float color[3];
    float scale;
    float posX, posY;
    float width, height;
};

