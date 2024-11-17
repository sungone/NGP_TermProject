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

// 바닥
extern Base base;

// 벽
extern Wall wall;

// 플레이어들
extern Player player;
extern map<int, ViewerPlayer*> viewerPlayer;

// 맵
extern CMap backgroundmap;

// 오브젝트들
extern vector<Object*> objects;

// 카메라
extern Camera camera;
extern CameraMode cameraMode;

// 화면
extern CImage screen;
extern GLuint windowWidth;
extern GLuint windowHeight;
extern bool full;

extern bool hpBarSet[2];
extern int hp;

// 충돌
extern bool plSizeChange;

// 조명
extern CLight light;
extern CLight light2;
extern CLight light3;

// gl 변수
extern GLclampf g_color[4];

// shader 변수
extern GLuint shaderProgramID;
extern glm::mat4 model;
extern glm::mat4 view;
extern glm::mat4 projection;

extern int wallUpdateSpeed;

//서버통신소켓 생성
extern Client client;