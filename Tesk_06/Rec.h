#pragma once
#include <glew.h>
#include <freeglut.h>
#include <freeglut_ext.h>

class Rec 
{
public:
    Rec();              // 랜덤 초기화
    void draw() const;

private:
    float color[3];
    float scale;
    float posX, posY;
    float width, height;
};

