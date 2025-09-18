#include "Eraser.h"
#include <glew.h>
#include <freeglut.h>

Rec eraser;         // 지우개
bool eraseMode = false;

// 지우개 생성
void InitEraser()
{
    eraser.color[0] = 0.0f;
    eraser.color[1] = 0.0f;
    eraser.color[2] = 0.0f;
    eraser.scale = 1.0f;
    eraser.width = recW * 2.0f;
    eraser.height = recH * 2.0f;
}

// 지우개 그리기
void DrawEraser()
{
    glPushMatrix();
    glTranslatef(eraser.posX, eraser.posY, 0.0f);  // 사각형의 중심을 posX, posY로 이동
    glScalef(eraser.scale, eraser.scale, 1.0f);
    glColor3fv(eraser.color);
    glRectf(
        -eraser.width / 2, -eraser.height / 2,
        eraser.width / 2, eraser.height / 2
    );
    glPopMatrix();
}
