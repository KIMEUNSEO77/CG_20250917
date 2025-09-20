#include <iostream>
#include <glew.h>
#include <freeglut.h>
#include <freeglut_ext.h>
#include <vector>
#include <random>
#include "Rec.h"
using namespace std;

int width = 800;
int height = 600;

vector<Rec> rects;  // 사각형들

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

// 5~10개의 사각형을 랜덤 위치, 색상으로 생성
void InitRects()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(5, 10);
    int rectCount = dis(gen);

    rects.clear();
    rects.reserve(rectCount);
    for (int i = 0; i < rectCount; ++i) 
    {
        rects.emplace_back(); // push_back이랑 반대로 바로 객체 생성
    }
}

void main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(width, height);
    glutCreateWindow("Tesk_06");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Unable to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "GLEW Initialized\n";

	InitRects();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutMainLoop();
}

GLvoid drawScene()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (const auto& r : rects) r.draw();

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}