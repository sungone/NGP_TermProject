// Global.h
#pragma once

#include "shaders.h"
#include "base.h"
#include "wall.h"
#include "player.h"
#include "Image.h"
#include "Light.h"
#include "Map.h"
#include "TextManager.h"
#include "Client.h"
#include "../Client/ViewerPlayer.h"

// �ٴ�
extern Base base;

// ��
extern Wall wall;

// �÷��̾��
extern Player player;
extern map<int, ViewerPlayer*> viewerPlayer;

// ��
extern CMap backgroundmap;

// ������Ʈ��
extern vector<Object*> objects;

// ī�޶�
extern Camera camera;
extern CameraMode cameraMode;

// ȭ��
extern CImage screen;
extern GLuint windowWidth;
extern GLuint windowHeight;
extern bool full;

extern bool hpBarSet[2];
extern int hp;

// �浹
extern bool plSizeChange;

// ����
extern CLight light;
extern CLight light2;
extern CLight light3;

// gl ����
extern GLclampf g_color[4];

// shader ����
extern GLuint shaderProgramID;
extern glm::mat4 model;
extern glm::mat4 view;
extern glm::mat4 projection;

extern int wallUpdateSpeed;

//������ż��� ����
extern Client client;