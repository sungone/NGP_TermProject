
#include "pch.h"
#include "shaders.h"
#include "base.h"
#include "wall.h"
#include "player.h"
#include "Image.h"
#include "Light.h"
#include "Map.h"
#include "TextManager.h"
#include "ThreadManager.h"
#include "ClientData.h"

/*****************************************
* 변수들 전역변수 공용으로 사용하기위해  *
* ServerCore 의 ClinetData 로 이동       *
******************************************/


/******************************************
            <Screen Status>
 0 : 메인 화면 1 : 인게임 스크린 (HP : 66 % ) 2 : 게임 승리 3 : 게임 오버 
 4 : 인게임 스크린 (HP 100) 5 : 인게임 스크린 (HP 33) 6 : 매칭 화면
******************************************/

// 초기화
void init();
void gameExit();
// gl 함수
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid keyboard(unsigned char key, int x, int y);
GLvoid KeyboardSpecial(int, int, int);
GLvoid Mouse(int button, int state, int x, int y);
// 벽 이동 함수
void wallUpdate();
// 게임 전체 플레이 로직
GLvoid update(int value);

void initCamera();

void main(int argc, char** argv)
{
	client.Init();

	ThreadManager::Launch([]()
		{
			client.PacketDecode();
		});

	ThreadManager::Launch([]()
		{
			client.Send();
		});

	client.SendConnectServer();

	PlaySound(L"sound/opening.wav", NULL, SND_ASYNC | SND_LOOP);//sound

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(500, 100);
	glutInitWindowSize(windowWidth, windowHeight);

	glutCreateWindow("Unity of Mind");
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "Game Start!" << std::endl;

	glEnable(GL_DEPTH_TEST);

	char vertexFile[] = "shader/vertex.glsl";
	char fragmentFile[] = "shader/fragment.glsl";
	shaderProgramID = initShader(vertexFile, fragmentFile);

	// 초기화
	TextManager::GetInstance()->Init();
	init();
	glutTimerFunc(wallUpdateSpeed, update, 50);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(KeyboardSpecial);
	glutMouseFunc(Mouse);
	glutMainLoop();
	atexit(gameExit);
}

GLvoid drawScene()
{

	glClearColor(g_color[0], g_color[1], g_color[2], g_color[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);

	glViewport(0, 0, windowWidth, windowHeight);

	// Camera
	camera.setCamera(shaderProgramID, 0, cameraMode, player.getPos());
	screen.render(shaderProgramID);

	// Object Draw
	if (1 == screen.status or 4 == screen.status or 5 == screen.status) {

		// 마우스 커서 숨기기
		ShowCursor(FALSE);

		backgroundmap.render(shaderProgramID);

		for (int i = 0; i < objects.size(); ++i)
			(*objects[i]).render(shaderProgramID);

	}

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	windowWidth = w;
	windowHeight = h;
	glViewport(0, 0, windowWidth, windowHeight);
}

GLvoid keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'z': // 플레이어 빨간색 변경
		player.changeRed();
		break;
	case 'x': // 플레이어 초록색 변경
		player.changeGreen();
		break;
	case 'c': // 플레이어 파란색 변경
		player.changeBlue();
		break;
	case 'v': // 플레이어 축소 / 확대
		player.changeSize();

		if (not plSizeChange)
			plSizeChange = true;
		else
			plSizeChange = false;
		break;
	case '1':
		cameraMode = FIRST_PERSON;
		camera.setEye(glm::vec3(player.getPos().x, camera.getEye().y, camera.getEye().z));
		break;
	case '3':
		cameraMode = THIRD_PERSON;
		break;

	case 27://esc : 프로그램 종료
		exit(-1);
		break;

	case '[': // Game start , Game over 테스트 하기위함

		if (1 == screen.status or 4 == screen.status or 5 == screen.status) 
		{
			screen.status = 2;
			PlaySound(L"sound/win.wav", NULL, SND_ASYNC | SND_LOOP);//sound
		}
		else if (2 == screen.status)
		{
			screen.status = 3;
			PlaySound(L"sound/closing.wav", NULL, SND_ASYNC | SND_LOOP);//sound
		}
		else if (3 == screen.status)
		{
			screen.status = 6;
		}
		else if (6 == screen.status)
		{
			screen.status = 1;
			PlaySound(L"sound/inGame.wav", NULL, SND_ASYNC | SND_LOOP);//sound
		}

		player.init();
		//player.setPos(vec3(0, 0, 0));

		camera.setCamera(shaderProgramID, 0, cameraMode, player.getPos());
		screen.initTex();

		break;
	}

	glutPostRedisplay();
}
GLvoid KeyboardSpecial(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:
		// 왼쪽 화살표 키 처리
		player.moveLeft();

		if (FIRST_PERSON == cameraMode)
			camera.moveLeft();
		break;
	case GLUT_KEY_RIGHT:
		// 오른쪽 화살표 키 처리
		player.moveRight();

		if (FIRST_PERSON == cameraMode)
			camera.moveRight();
		break;

	case GLUT_KEY_CTRL_L:
	case GLUT_KEY_CTRL_R://전체 화면
		// Ctrl 키 처리
		glutFullScreenToggle();

		if (not full)
			full = true;
		else
			full = false;
		break;
	}
}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		/*cout << "x : " << x << endl;
		cout << "y : " << y << endl << endl << endl;*/

		if (0 == screen.status)
		{
			if (not full)
			{
				if (513 <= x && 616 >= x and 528 <= y && 583 >= y) {//play
					screen.status = 1;
					screen.initTex();
					PlaySound(L"sound/inGame.wav", NULL, SND_ASYNC | SND_LOOP);//sound
				}
				else if (507 <= x && 603 >= x and 595 <= y && 648 >= y)//gameExit
					exit(-1);

				else if (505 <= x && 673 >= x and 663 <= y && 711 >= y)
				{
					// 임시로 로비 입장 버튼이 setting 버튼으로 해놓음
					screen.status = 6;
					cout << "Lobby Enter" << endl;
					client.SendMatchingStart();
					screen.initTex();
					PlaySound(L"sound/inGame.wav", NULL, SND_ASYNC | SND_LOOP);//sound
				}
			}
			else
			{
				if (1242 <= x && 1490 >= x and 719 <= y && 786 >= y) {//play
					screen.status = 1;
					screen.initTex();
					PlaySound(L"sound/inGame.wav", NULL, SND_ASYNC | SND_LOOP);//sound
				}

				else if (1228 <= x && 1446 >= x and 807 <= y && 873 >= y)
				{//gameExit
					exit(-1);
				}

				else if (1216 <= x && 1618 >= x and 899 <= y && 957 >= y)
				{
					// 임시로 로비 입장 버튼이 setting 버튼으로 해놓음
				/*	client.SendMatchingStart();*/
					screen.status = 6;
					screen.initTex();
					PlaySound(L"sound/inGame.wav", NULL, SND_ASYNC | SND_LOOP);//sound
				}
			}
		}

		else if (screen.status == 6) // 매칭 화면 일때 - 3명을 기다리는 방
		{
			if (not full)
			{
				if (711 <= y && 799 >= y and 300 <= x && 500 >= x) // 매칭 취소 버튼 설정
				{
					client.SendMatchingCancel();
					screen.status = 0;
					screen.initTex();
					PlaySound(L"sound/opening.wav", NULL, SND_ASYNC | SND_LOOP);//sound
				}
			}
		}
	}
}

void init()
{
	initCamera();

	base.init();
	base.initBuffer();
	base.initTexture();
	objects.push_back(&base);

	wall.init();
	objects.push_back(&wall);

	player.init();
	objects.push_back(&player);

	//for (int i = 0; i < 3; i++)
	//{
	//	viewerPlayers[i].init();
	//	viewerPlayers[i].setPosX(-0.15f + 0.1f * i);
	//	objects.push_back(&viewerPlayers[i]);
	//}

	for (auto it = viewerPlayer.begin(); it != viewerPlayer.end(); ++it)
	{
		it->second->init();
		it->second->setPosX(-0.15f + 0.1f);
		objects.push_back(it->second);
	}
	
	//vPlayer1.init();
	//vPlayer1.setPosX(0.15f);
	//objects.push_back(&vPlayer1);

	//vPlayer2.init();
	//vPlayer2.setPosX(-0.15f);
	//objects.push_back(&vPlayer2);

	backgroundmap.init();


	screen.initBuf();
	screen.initTex();

	light.InitBuffer(shaderProgramID, camera);
}
void gameExit()
{
	cout << "Game gameExit" << endl;
	client.SendMatchingCancel();
	if (screen.status == 6)
	{
	}
}


void initCamera()
{
	camera.setWinSize(windowWidth, windowHeight);

	camera.setFovy(45.0f);
	camera.setzNear(0.1f);
	camera.setzFar(50.0f);

	camera.setLeft(-2.0f);
	camera.setRight(2.0f);
	camera.setBottom(-2.0f);
	camera.setTop(2.0f);

	camera.setYaw(-90.f);
	camera.setPitch(-20.f);
	camera.setAngle(-45.f);

	camera.setEye(glm::vec3(0.f, 1.f, 2.f));
}

GLvoid update(int value)
{

	if (1 == screen.status or 4 == screen.status or 5 == screen.status)
		wallUpdate();

	glutTimerFunc(wallUpdateSpeed, update, value);
	glutPostRedisplay();
}


void wallUpdate()
{
	wall.moveWall(); // 벽 움직이는 함수

	if (30 == wall.cur_idx) // 게임 승리
	{
		screen.status = 2;
		PlaySound(L"sound/win.wav", NULL, SND_ASYNC | SND_LOOP);//sound

		player.init();
		camera.setCamera(shaderProgramID, 0, cameraMode, player.getPos());
		screen.initTex();
	}

	if (!wall.emptyIdx.empty()) // 충돌처리 하는 로직이 들어있음
	{
		if ((!player.crashOnce) and 1.25f < wall.getCube(wall.emptyIdx[0].x, wall.emptyIdx[0].y).getPos().z) {
			int findIndex = -1;
			for (int i{}; i < wall.emptyIdx.size(); ++i) {
				if ((player.getPos().x - 0.13f >= wall.emptyIdx[i].y * 0.3333f - 0.5f && player.getPos().x + 0.13f <= wall.emptyIdx[i].y * 0.3333f + 0.3333f - 0.5f))// ||
					//(player.getPos().x + 0.13f >= wall.emptyIdx[i].y * 0.3333f - 0.5f && player.getPos().x <= wall.emptyIdx[i].y * 0.3333f + 0.3333f - 0.5f))
				{
					findIndex = i;
					break;
				}
			}


			if (findIndex != -1)
			{
				if (0 == (wall.cur_idx - 1) / 10) {
					client.BlockCollision();
				}
				else if (1 == (wall.cur_idx - 1) / 10) {
					if (not(1 == player.getColor().x and 0 == wall.emptyIdx[findIndex].x
						or 1 == player.getColor().y and 1 == wall.emptyIdx[findIndex].x
						or 1 == player.getColor().z and 2 == wall.emptyIdx[findIndex].x)) {
						client.BlockCollision();
					}
				}
				else if (2 == (wall.cur_idx - 1) / 10) {
					if (0 == wall.emptyIdx[findIndex].x and not plSizeChange
						or 1 == wall.emptyIdx[findIndex].x) {
						client.BlockCollision();
					}

				}
			}
		}
	}

	if (not wall.emptyIdx.empty() and 1.3f < wall.getCube(wall.emptyIdx[0].x, wall.emptyIdx[0].y).getPos().z)
	{
		
	}
}













