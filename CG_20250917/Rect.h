#pragma once
#include <vector>

struct Rec
{
    float color[3];
    float scale;
    float posX, posY;
    float width, height;

    float vx, vy; // ¼Óµµ
};

void RandomColor(float color[3]);
void RandomPosition(float& posX, float& posY);
void DrawRect(const Rec& rect);
void InitRects();
void AddRect(); 
void MoveRect(Rec& rect, float dx, float dy);
bool IsWall(const Rec& rec, float x, float y);

extern float recW;
extern float recH;
extern bool rectMove;

extern std::vector<Rec> rects;
