#pragma once
#include "object.h"
#include "FileIn.h"

typedef class Player : public Object
{
private:

	glm::mat4 pModel; // player ��



	void drawP(GLuint shaderProgramID);
public:
	float GetTextPos() { return TextPos; }
	bool crashOnce{};
	CFileIn in;
	float TextPos = -0.05f;

	
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
