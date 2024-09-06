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
	void reset();
	bool moveCube();

	// 1 스테이지 - 랜덤값의 나머지가 0이면 뚫림
	void changeEmpty();

	// 2 스테이지 - 나머지가 0 , 1 , 2 면 각각 r , g , b
	void changeRed();
	void changeGreen();
	void changeBlue();

	// 3 스테이지 - 나머지가 0 이면 사이즈 줄임
	void change_SmallSize();

} Cube;

typedef class Wall : public Cube
{
private:
	Cube cube[3][3];
	int	random_num[36][3];

public:
	vector<vec2> emptyIdx /*= { {0,0},{0,1},{0,2} }*/;
	int crashCnt{};
	int cur_idx{};
public:
	void init();
	void render(GLuint shaderProgramID) override;
	Cube getCube(int i, int j);

	void moveWall();

	// 1 , 2 , 3 스테이지를 통합
	void makeWall(int i, int j, int cur_idx);

}Wall;



