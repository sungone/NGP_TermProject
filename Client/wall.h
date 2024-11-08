#pragma once
#include "object.h"

typedef class Cube : public Object
{
private:
	glm::mat4 cModel; 
	glm::vec3 color;

public:
	void init() override;
	void render(GLuint shaderProgramID) override;
	void drawCube(GLuint shaderProgramID);

	// 큐브를 움직이고 큐브가 끝에 도착하면 다시 처음 자리로 리셋해주는 함수
	void reset();
	bool moveCube();
	bool moveFinal();
	 
	// 1 스테이지를 깨기위해 필요한 블럭을 없애는 함수 - x값을 멀리 보내서 화면에 안보이게 함 (랜덤값의 나머지가 0이면 블럭을 없앰)
	void changeEmpty();

	// 2 스테이지 블럭의 색깔을 바꾸는 함수 (랜덤값의 나머지가 0 , 1 , 2 면 각각 r , g , b)
	void changeRed();
	void changeGreen();
	void changeBlue();

	// 3 스테이지 블럭을 작게 만들어주는 함수 (랜덤값의 나머지가 0 이면 사이즈 줄임)
	void change_SmallSize();

} Cube;

typedef class Wall : public Cube
{
private:
	Cube cube[3][3];
	int	random_num[36][3];

public:
	vector<vec2> emptyIdx /*= { {0,0},{0,1},{0,2} }*/;
	int cur_idx{};
public:
	void init();
	void render(GLuint shaderProgramID) override;
	Cube getCube(int i, int j);

	// 벽을 움직이는 함수들 , 게임을 플레이하는데 핵심 함수
	// 맨 처음 오는 벽은 그냥 게임을 시작했다는 것을 알리기 위해서 충돌하지않고 그냥 지나감
	// 1스테이지 (10wave) - 비어있는 공간으로 피함
	// 2스테이지 (10wave) - 벽의 색과 플레이어의 색을 똑같이 바꿔서 충돌을 피함
	// 3스테이지 (10wave) - 플레이어를 작게 만들어 충돌을 피함
	void moveWall();
	void makeWall(int i, int j, int cur_idx);

}Wall;



