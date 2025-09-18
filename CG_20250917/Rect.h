#pragma once
#include <vector>

struct Rec
{
    float color[3];
    float scale;
    float posX, posY;
    float width, height;
};

void RandomColor(float color[3]);
void RandomPosition(float& posX, float& posY);
void DrawRect(const Rec& rect);
void InitRects();

extern float recW;
extern float recH;

extern std::vector<Rec> rects;
