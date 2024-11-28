#pragma once
#include "object.h"
#include "FileIn.h"

typedef class Player : public Object
{
private:

	glm::mat4 pModel; // player ��



	void drawP(GLuint shaderProgramID);
public:
	float HelloWorld() { return sum; }
	bool crashOnce{};
	CFileIn in;
	float sum{};

	
	
public:

	void init() override;
	void render(GLuint shaderProgramID) override;
	void initBuf();

	// �÷��̾� �̵�
	void moveLeft();
	void moveRight();

	//�÷��̾� �� ����
	void changeRed();
	void changeGreen();
	void changeBlue();

	// �÷��̾� ũ�� ����
	void changeSize();
};
