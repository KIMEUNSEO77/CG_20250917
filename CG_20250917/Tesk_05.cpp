#include <iostream>
#include <glew.h>
#include <freeglut.h>
#include <freeglut_ext.h>
#include <vector>
#include <random>
using namespace std;

int width = 800;
int height = 600;

float recW = 0.06f;  // 사각형의 너비
float recH = 0.08f;  // 사각형의 높이

struct Rec
{
    float color[3];
    float scale;
    float posX, posY;
    float width, height;
};
vector<Rec> rects;  // 사각형들
Rec eraser;         // 지우개

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

// 20~40개의 사각형을 랜덤 위치, 색상으로 생성
void InitRects()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(20, 40);
    int rectCount = dis(gen);
    for (int i = 0; i < rectCount; ++i)
    {
        Rec temp{};
        RandomPosition(temp.posX, temp.posY);
        RandomColor(temp.color);
        temp.scale = 1.0f;
        temp.width = recW;
        temp.height = recH;
        rects.push_back(temp);
    }
}

// 마우스 클릭한 좌표 정규화
void PixelTrans(int px, int py, float& nx, float& ny)
{
    float w = (float)glutGet(GLUT_WINDOW_WIDTH);
    float h = (float)glutGet(GLUT_WINDOW_HEIGHT);
    nx = (px / w) * 2.0f - 1.0f;
    ny = 1.0f - (py / h) * 2.0f;
}

// 중심 좌표 화면 밖으로 못 나가게
void MouseInside(Rec& r)
{
    float w = (r.width * r.scale) * 0.5f;
    float h = (r.height * r.scale) * 0.5f;

    if (r.posX < -1.0f + w) r.posX = -1.0f + w;
    if (r.posX > 1.0f - w) r.posX = 1.0f - w;
    if (r.posY < -1.0f + h) r.posY = -1.0f + h;
    if (r.posY > 1.0f - h) r.posY = 1.0f - h;
}

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

// 마우스 콜백
void Mouse(int button, int state, int x, int y)
{
    float nx, ny;  // 마우스가 클릭한 좌표를 정규화
    PixelTrans(x, y, nx, ny);

    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
			eraser.posX = nx;
			eraser.posY = ny;
			MouseInside(eraser);
			InitEraser();
            glutPostRedisplay(); // 화면 갱신
        }
        else if (state == GLUT_UP)
        {
            
        }
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
       
    }
    glutPostRedisplay();
}

void main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(width, 600);
    glutCreateWindow("Tesk_05");

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

    glutMouseFunc(Mouse);

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
    DrawEraser();

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}