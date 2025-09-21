#pragma once
#include <glew.h>
#include <freeglut.h>
#include <freeglut_ext.h>

struct Rec
{
    float color[3];
    float scale;
    float posX, posY;
    float width, height;
};

void DrawRect(const Rec& rect);
void InitRec(Rec& rect);
