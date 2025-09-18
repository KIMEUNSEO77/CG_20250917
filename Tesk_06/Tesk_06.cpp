#include <iostream>
#include <glew.h>
#include <freeglut.h>
#include <freeglut_ext.h>
#include <vector>
#include <random>
using namespace std;

int width = 800;
int height = 600;

struct Rec
{
    float color[3];
    float scale;
    float posX, posY;
    float width, height;
};
vector<Rec> rects;  // 사각형들

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

// 랜덤 색상 생성
void RandomColor(float color[3])
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.01f, 1.0f);
    color[0] = dis(gen);
    color[1] = dis(gen);
    color[2] = dis(gen);
}

// 랜덤 위치 생성
void RandomPosition(float& posX, float& posY)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    posX = dis(gen);
    posY = dis(gen);
}

// 랜덤 크기 설정
void RandomSize(float& width, float& height)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.1f, 0.3f);

    width = dis(gen);
    height = dis(gen);
}

// 사각형 그리기
void DrawRect(const Rec& rect)
{
    glPushMatrix();
    glTranslatef(rect.posX, rect.posY, 0.0f);  // 사각형의 중심을 posX, posY로 이동
    glScalef(rect.scale, rect.scale, 1.0f);
    glColor3fv(rect.color);
    glRectf(
        -rect.width / 2, -rect.height / 2,
        rect.width / 2, rect.height / 2
    );
    glPopMatrix();
}

// 5~10개의 사각형을 랜덤 위치, 색상으로 생성
void InitRects()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(5, 10);
    int rectCount = dis(gen);
    for (int i = 0; i < rectCount; ++i)
    {
        Rec temp{};
        RandomPosition(temp.posX, temp.posY);
        RandomColor(temp.color);
        temp.scale = 1.0f;
		RandomSize(temp.width, temp.height);
        rects.push_back(temp);
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

    for (int i = 0; i < rects.size(); ++i)
    {
        DrawRect(rects[i]);
	}

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}