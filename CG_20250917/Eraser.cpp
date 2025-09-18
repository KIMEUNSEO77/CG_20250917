#include "Eraser.h"
#include <glew.h>
#include <freeglut.h>

Rec eraser;         // ���찳
bool eraseMode = false;

// ���찳 ����
void InitEraser()
{
    eraser.color[0] = 0.0f;
    eraser.color[1] = 0.0f;
    eraser.color[2] = 0.0f;
    eraser.scale = 1.0f;
    eraser.width = recW * 2.0f;
    eraser.height = recH * 2.0f;
}

// ���찳 �׸���
void DrawEraser()
{
    glPushMatrix();
    glTranslatef(eraser.posX, eraser.posY, 0.0f);  // �簢���� �߽��� posX, posY�� �̵�
    glScalef(eraser.scale, eraser.scale, 1.0f);
    glColor3fv(eraser.color);
    glRectf(
        -eraser.width / 2, -eraser.height / 2,
        eraser.width / 2, eraser.height / 2
    );
    glPopMatrix();
}
