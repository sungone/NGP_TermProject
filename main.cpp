#include "shaders.h"
#include "base.h"
#include "wall.h"
#include "player.h"
#include "Image.h"
#include "Light.h"
#include "Map.h"


// 바닥
Base base;

// 벽
Wall wall;

// 플레이어
Player player;

//맵
CMap map;

// 오브젝트들
vector<Object*> objects;



// 카메라
Camera camera;
void initCamera();
CameraMode cameraMode{ THIRD_PERSON };


//화면
CImage screen;
GLuint windowWidth{ 800 };
GLuint windowHeight{ 800 };
bool full{};
bool hpBarSet[2]{};

//충돌
bool plSizeChange{};

//조명
CLight light;

// 초기화
void init();

// gl 변수
GLclampf g_color[4] = { 0.f, 0.f, 0.f, 1.f };


// gl 함수
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid keyboard(unsigned char key, int x, int y);
GLvoid KeyboardSpecial(int, int, int);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid update(int value);


// shader 변수
GLuint shaderProgramID;
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

// 벽 이동 함수
void wallUpdate();
int wallUpdateSpeed = 20;




void main(int argc, char** argv)
{
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

	char vertexFile[] = "vertex.glsl";
	char fragmentFile[] = "fragment.glsl";
	shaderProgramID = initShader(vertexFile, fragmentFile);

	// 초기화
	init();


	glutTimerFunc(wallUpdateSpeed, update, 50);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(KeyboardSpecial);
	glutMouseFunc(Mouse);
	glutMainLoop();
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

		map.render(shaderProgramID);

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

	case '[':

		if (1 == screen.status or 4 == screen.status or 5 == screen.status) {
			screen.status = 2;

			PlaySound(L"sound/win.wav", NULL, SND_ASYNC | SND_LOOP);//sound
		}
		else if (2 == screen.status) {
			screen.status = 3;

			PlaySound(L"sound/closing.wav", NULL, SND_ASYNC | SND_LOOP);//sound
		}
		else if (3 == screen.status) {
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

		if (0 == screen.status) {
			if (not full) {
				if (513 <= x && 616 >= x and 528 <= y && 583 >= y) {//play
					screen.status = 1;
					screen.initTex();
					PlaySound(L"sound/inGame.wav", NULL, SND_ASYNC | SND_LOOP);//sound
				}
				else if (507 <= x && 603 >= x and 595 <= y && 648 >= y)//exit
					exit(-1);
				else if (505 <= x && 673 >= x and 663 <= y && 711 >= y) {//settings

				}
			}
			else {
				if (1242 <= x && 1490 >= x and 719 <= y && 786 >= y) {//play
					screen.status = 1;
					screen.initTex();
					PlaySound(L"sound/inGame.wav", NULL, SND_ASYNC | SND_LOOP);//sound
				}
				else if (1228 <= x && 1446 >= x and 807 <= y && 873 >= y)//exit
					exit(-1);
				else if (1216 <= x && 1618 >= x and 899 <= y && 957 >= y) {//settings

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
	map.init();


	screen.initBuf();
	screen.initTex();

	light.InitBuffer(shaderProgramID, camera);
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
	wall.moveWall();


	if (30 == wall.cur_idx) {
		screen.status = 2;
		PlaySound(L"sound/win.wav", NULL, SND_ASYNC | SND_LOOP);//sound

		player.init();
		camera.setCamera(shaderProgramID, 0, cameraMode, player.getPos());
		screen.initTex();
	}
	else if (3 == wall.crashCnt) {
		screen.status = 3;

		PlaySound(L"sound/closing.wav", NULL, SND_ASYNC | SND_LOOP);//sound

		player.init();
		camera.setCamera(shaderProgramID, 0, cameraMode, player.getPos());
		screen.initTex();
	}
	else if (2 == wall.crashCnt and !hpBarSet[1]) {
		screen.status = 5;
		screen.initTex();
		hpBarSet[1] = true;
	}
	else if (1 == wall.crashCnt and !hpBarSet[0]) {
		screen.status = 4;
		screen.initTex();
		hpBarSet[0] = true;
	}

	if (not wall.emptyIdx.empty()) {
		if (not player.crashOnce and 1.25f < wall.getCube(wall.emptyIdx[0].x, wall.emptyIdx[0].y).getPos().z) {
			for (int i{}; i < wall.emptyIdx.size(); ++i) {
				if ((player.getPos().x/*-0.01f*/ >= wall.emptyIdx[i].y * 0.3333f - 0.5f
					and player.getPos().x + 0.13f <= wall.emptyIdx[i].y * 0.3333f + 0.3333f - 0.5f)//pl==cube

					or (player.getPos().x + 0.13f > wall.emptyIdx[i].y * 0.3333f - 0.5f
						and player.getPos().x/*-0.01f*/ < wall.emptyIdx[i].y * 0.3333f + 0.3333f - 0.5f)) {

					if (0 == (wall.cur_idx - 1) / 10) {

						++wall.crashCnt;
						player.crashOnce = true;

						//cout << "1 crash : " << wall.crashCnt << endl;
						//cout << "1 size : " << wall.emptyIdx.size() << endl;
						break;
					}
					else if (1 == (wall.cur_idx - 1) / 10) {
						if (not(1 == player.getColor().x and 0 == wall.emptyIdx[i].x
							or 1 == player.getColor().y and 1 == wall.emptyIdx[i].x
							or 1 == player.getColor().z and 2 == wall.emptyIdx[i].x)) {

							++wall.crashCnt;
							player.crashOnce = true;

							//cout << "2 crash : " << wall.crashCnt << endl;
							//cout << "2 size : " << wall.emptyIdx.size() << endl;
							break;
						}
						/*else {
							cout << "2 color x" << endl << endl;
						}*/
					}
					else if (2 == (wall.cur_idx - 1) / 10) {
						if (0 == wall.emptyIdx[i].x and not plSizeChange
							or 1 == wall.emptyIdx[i].x) {

							++wall.crashCnt;
							player.crashOnce = true;

							//cout << "3 crash : " << wall.crashCnt << endl;
							//cout << "3 size : " << wall.emptyIdx.size() << endl;
							break;
						}
						/*else {
							cout << "3 size x" << endl << endl;
						}*/
					}
				}
				//else if (0 == (wall.cur_idx - 1) / 10) {
				//	cout << "1 pos x" << endl << endl;
				//}
				//else if (1 == (wall.cur_idx - 1) / 10) {
				//	cout << "2 pos x" << endl << endl;//
				//}
				//else if (2 == (wall.cur_idx - 1) / 10) {
				//	cout << "3 pos x" << endl << endl;//
				//}
			}
		}
	}

	if (not wall.emptyIdx.empty() and 1.3f < wall.getCube(wall.emptyIdx[0].x, wall.emptyIdx[0].y).getPos().z) {

		player.crashOnce = false;
		wall.emptyIdx.clear();
	}
}













