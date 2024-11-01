#include "pch.h"
#include "ClientData.h"

// 바닥
Base base;

// 벽
Wall wall;

// 플레이어
Player player;

// 맵
CMap backgroundmap;

// 오브젝트들
vector<Object*> objects;

// 카메라
Camera camera;
CameraMode cameraMode{ THIRD_PERSON };

// 화면
CImage screen;
GLuint windowWidth{ 800 };
GLuint windowHeight{ 800 };
bool full{};
bool hpBarSet[2]{};

// 충돌
bool plSizeChange{};

// 조명
CLight light;
CLight light2;
CLight light3;

// gl 변수
GLclampf g_color[4] = { 0.f, 0.f, 0.f, 1.f };

// shader 변수
GLuint shaderProgramID;
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

int wallUpdateSpeed = 20;
