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

	// ť�긦 �����̰� ť�갡 ���� �����ϸ� �ٽ� ó�� �ڸ��� �������ִ� �Լ�
	void reset();
	bool moveCube();
	bool moveFinal();
	 
	// 1 ���������� �������� �ʿ��� ���� ���ִ� �Լ� - x���� �ָ� ������ ȭ�鿡 �Ⱥ��̰� �� (�������� �������� 0�̸� ���� ����)
	void changeEmpty();

	// 2 �������� ���� ������ �ٲٴ� �Լ� (�������� �������� 0 , 1 , 2 �� ���� r , g , b)
	void changeRed();
	void changeGreen();
	void changeBlue();

	// 3 �������� ���� �۰� ������ִ� �Լ� (�������� �������� 0 �̸� ������ ����)
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

	// ���� �����̴� �Լ��� , ������ �÷����ϴµ� �ٽ� �Լ�
	// �� ó�� ���� ���� �׳� ������ �����ߴٴ� ���� �˸��� ���ؼ� �浹�����ʰ� �׳� ������
	// 1�������� (10wave) - ����ִ� �������� ����
	// 2�������� (10wave) - ���� ���� �÷��̾��� ���� �Ȱ��� �ٲ㼭 �浹�� ����
	// 3�������� (10wave) - �÷��̾ �۰� ����� �浹�� ����
	void moveWall();
	void makeWall(int i, int j, int cur_idx);

}Wall;



