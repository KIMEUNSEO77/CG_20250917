#pragma once
#include <glew.h>
#include <freeglut.h>
#include <freeglut_ext.h>
#include <array>

class Rec 
{
    // const: 특정 멤버 함수가 객체 상태를 바꾸지 않음을 보장
public:
    Rec();                                      // 랜덤 초기화
    void draw() const;
	bool HitRect(float nx, float ny) const;     // 이 사각형이 클릭되었는지
    // 크기 고정이라 vector대신 array로...
	std::array<Rec, 4> MakeChild() const;       // 4등분
	std::array<Rec, 8> MakeDoubleChild() const; // 8등분

	float getPosX() const { return posX; }
	float getPosY() const { return posY; }
	float getWidth() const { return width; }
	float getHeight() const { return height; }
	float getScale() const { return scale; }

    // 애니메이션용 변수
    float vx = 0.0f, vy = 0.0f; // 속도
    bool moving = false;

    // 갱신용
    void SetPos(float x, float y) { posX = x; posY = y; }
	void SetScale(float s) { scale = s; }
    bool tooSmall() const { return (scale <= 0.25f); } // 너무 작아졌는지

private:
    float color[3];
    float scale;
    float posX, posY;
    float width, height;
};

