#include "pch.h"
#include "ClientData.h"

// �ٴ�
Base base;

// ��
Wall wall;

// �÷��̾�
Player player;

// ��
CMap backgroundmap;

// ������Ʈ��
vector<Object*> objects;

// ī�޶�
Camera camera;
CameraMode cameraMode{ THIRD_PERSON };

// ȭ��
CImage screen;
GLuint windowWidth{ 800 };
GLuint windowHeight{ 800 };
bool full{};
bool hpBarSet[2]{};

// �浹
bool plSizeChange{};

// ����
CLight light;
CLight light2;
CLight light3;

// gl ����
GLclampf g_color[4] = { 0.f, 0.f, 0.f, 1.f };

// shader ����
GLuint shaderProgramID;
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

int wallUpdateSpeed = 20;

//������ż��� ����
Client client;