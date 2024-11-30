



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
#include "Client.h"
#include "TimeManager.h"


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
GLvoid update();

void initCamera();
HWND hwnd;

uint64 lastTick = 0;

void main(int argc, char** argv)
{
	client.Init();
	TimeManager::GetInstance()->Init();


	ThreadManager::Launch([]()
		{
			client.PacketDecode();
		});


	client.SendConnect();

	PlaySound(L"sound/opening.wav", NULL, SND_ASYNC | SND_LOOP);//sound

	glutInit(&argc, argv);
	::glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 컬러모델, 윈도우 버퍼 등 초기의 출력 모드를 결정한다.
	glutInitWindowPosition(300, 200);
	glutInitWindowSize(windowWidth, windowHeight);

	windowWidth = glutGet(GLUT_SCREEN_WIDTH); // 모니터의 가로 해상도 가져오기
	windowHeight = glutGet(GLUT_SCREEN_HEIGHT); // 모니터의 세로 해상도 가져오기

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

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(KeyboardSpecial);
	glutMouseFunc(Mouse);
	glutCloseFunc(gameExit);
	glutMainLoop();

}

GLvoid drawScene()
{

	TimeManager::GetInstance()->Update();

	glClearColor(g_color[0], g_color[1], g_color[2], g_color[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);

	glViewport(0, 0, windowWidth, windowHeight);

	update();
	// Camera
	camera.setCamera(shaderProgramID, 0, cameraMode, player.getPos());
	screen.render(shaderProgramID);

	// Object Draw
	if (E::HP100 == screen.status or E::HP33 == screen.status or E::HP66 == screen.status) {

		// 마우스 커서 숨기기
	/*	ShowCursor(FALSE);*/

		backgroundmap.render(shaderProgramID);

		for (int i = 0; i < objects.size(); ++i)
			(*objects[i]).render(shaderProgramID);

		float x = player.GetTextPos();
		glUseProgram(0); //unbind
		TextManager::GetInstance()->Render(x, -0.4f, "me");
	}


	glutSwapBuffers();
	glutPostRedisplay();
};

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
		client.SendPlayerInfo();
		break;
	case 'x': // 플레이어 초록색 변경
		player.changeGreen();
		client.SendPlayerInfo();
		break;
	case 'c': // 플레이어 파란색 변경
		player.changeBlue();
		client.SendPlayerInfo();
		break;
	case 'v': // 플레이어 축소 / 확대
		player.changeSize();
		client.SendPlayerInfo();
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

	case 13:
		if (screen.status == E::GAMEOVER)
		{
			screen.status = E::Main;
			client.GameOver();
			hp = 0;
			init();
			break;
		}
	case 27://esc : 프로그램 종료
		exit(-1);
		break;

	case '[': // Game start, Game over test
		if (screen.status == E::HP66 || screen.status == E::HP100 || screen.status == E::HP33)
		{
			screen.status = E::WIN;
			PlaySound(L"sound/win.wav", NULL, SND_ASYNC | SND_LOOP);
		}
		else if (screen.status == E::WIN)
		{
			screen.status = E::GAMEOVER;
			PlaySound(L"sound/closing.wav", NULL, SND_ASYNC | SND_LOOP);
		}
		else if (screen.status == E::GAMEOVER)
		{
			screen.status = E::MATCHING;
		}
		else if (screen.status == E::MATCHING)
		{
			screen.status = E::HP100;
			PlaySound(L"sound/inGame.wav", NULL, SND_ASYNC | SND_LOOP);
		}

		player.init();
		camera.setCamera(shaderProgramID, 0, cameraMode, player.getPos());
		screen.initTex();

		break;
	}

}
GLvoid KeyboardSpecial(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:
		// 왼쪽 화살표 키 처리
		player.moveLeft();

		client.SendPlayerInfo();
		if (FIRST_PERSON == cameraMode)
			camera.moveLeft();
		break;
	case GLUT_KEY_RIGHT:
		// 오른쪽 화살표 키 처리
		player.moveRight();
		client.SendPlayerInfo();
		if (FIRST_PERSON == cameraMode)
			camera.moveRight();
		break;
	case GLUT_KEY_CTRL_L:
		break;
	case GLUT_KEY_CTRL_R://전체 화면
		// Ctrl 키 처리
		glutFullScreenToggle();
		break;
	}
}

GLvoid Mouse(int button, int state, int x, int y)
{

	float normalizedX = static_cast<float>(x) / windowWidth;
	float normalizedY = static_cast<float>(y) / windowHeight;

	if (button == GLUT_LEFT_BUTTON)
	{
		if (screen.status == E::Main)
		{
			if (normalizedX >= 0.65 && normalizedX <= 0.77 &&
				normalizedY >= 0.66 && normalizedY <= 0.74)
			{
				screen.status = E::HP100;
				screen.initTex();
				PlaySound(L"sound/inGame.wav", NULL, SND_ASYNC | SND_LOOP);
			}

			else if (normalizedX >= 0.63 && normalizedX <= 0.75 &&
				normalizedY >= 0.744 && normalizedY <= 0.81)
			{
				exit(-1);
			}

			else if (normalizedX >= 0.62 && normalizedX <= 0.85 &&
				normalizedY >= 0.83 && normalizedY <= 0.9)
			{
				screen.status = E::MATCHING;
				std::cout << "Lobby Enter" << std::endl;
				client.SendStartGame();
				screen.initTex();
				PlaySound(L"sound/inGame.wav", NULL, SND_ASYNC | SND_LOOP);
			}
		}

		else if (screen.status == E::MATCHING)
		{
			if (normalizedX >= 0.313 && normalizedX <= 0.75 &&
				normalizedY >= 0.889 && normalizedY <= 0.999)
			{
				client.SendMatchingCancel();
				screen.status = E::Main;
				screen.initTex();
				PlaySound(L"sound/opening.wav", NULL, SND_ASYNC | SND_LOOP);
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

	client.InitViewerPlayer(client._clientID);

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

	std::cout << "Game Exit" << std::endl;
	if (screen.status == E::MATCHING)
	{
		client.SendMatchingCancel();
	}
	client.DisConnectClient();

	Sleep(100);
	closesocket(client.GetSokcet());
	WSACleanup();


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

GLvoid update()
{
	if (1 == screen.status or 4 == screen.status or 5 == screen.status)
		wallUpdate();

	if (hp >= 3) // hp >= 3 이면 hp 가 모두 소진되었으므로 게임 종료
	{
		screen.status = E::GAMEOVER;
		screen.initTex();
	}


}

void wallUpdate()
{
	wall.moveWall(); // 벽 움직이는 함수

	if (hp == 0) // Client 프로젝트의 main 에서만 씬 이동이 되는것 같아서 여기서 hp 로 씬 전환을 함 , hp >= 3 이면 update 에서!
	{
		screen.status = E::HP100;
		screen.initTex();
	}
	else if (hp == 1)
	{
		screen.status = E::HP66;
		screen.initTex();
	}
	else if (hp == 2)
	{
		screen.status = E::HP33;
		screen.initTex();
	}


	if (wall.cur_idx == 30) // Game win
	{
		screen.status = E::WIN;
		PlaySound(L"sound/win.wav", NULL, SND_ASYNC | SND_LOOP);

		player.init();
		camera.setCamera(shaderProgramID, 0, cameraMode, player.getPos());
		screen.initTex();
	}

	if (not wall.emptyIdx.empty()) // 충돌처리 하는 로직이 들어있음
	{
		if (not player.crashOnce and 1.25f < wall.getCube(wall.emptyIdx[0].x, wall.emptyIdx[0].y).getPos().z) {
			for (int i{}; i < wall.emptyIdx.size(); ++i) {
				if ((player.getPos().x/*-0.01f*/ >= wall.emptyIdx[i].y * 0.3333f - 0.5f
					and player.getPos().x + 0.13f <= wall.emptyIdx[i].y * 0.3333f + 0.3333f - 0.5f)//pl==cube

					or (player.getPos().x + 0.13f > wall.emptyIdx[i].y * 0.3333f - 0.5f
						and player.getPos().x/*-0.01f*/ < wall.emptyIdx[i].y * 0.3333f + 0.3333f - 0.5f)) {

					// cur_idx 1 ~ 9 1 단계
					if (0 == (wall.cur_idx) / 10) {

						++wall.crashCnt;
						player.crashOnce = true;
						client.BlockCollision();
						break;
					}
					else if (1 == (wall.cur_idx) / 10) // cur_idx 10 ~ 19 2단계
					{
						if (not(1 == player.getColor().x and 0 == wall.emptyIdx[i].x
							or 1 == player.getColor().y and 1 == wall.emptyIdx[i].x
							or 1 == player.getColor().z and 2 == wall.emptyIdx[i].x)) {

							++wall.crashCnt;
							player.crashOnce = true;
							client.BlockCollision();
							break;
						}
					}
					else if (2 == (wall.cur_idx) / 10) { // cur_idx 20 ~ 29 3단계
						if (0 == wall.emptyIdx[i].x and not plSizeChange
							or 1 == wall.emptyIdx[i].x) {

							++wall.crashCnt;
							player.crashOnce = true;
							client.BlockCollision();
							break;
						}

					}
				}

			}
		}
	}

	if (not wall.emptyIdx.empty() and 1.3f < wall.getCube(wall.emptyIdx[0].x, wall.emptyIdx[0].y).getPos().z)
	{
		//player.crashOnce = false;
		//wall.emptyIdx.clear();
	}
}













