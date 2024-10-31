#pragma once
#include "object.h"
#include "FileIn.h"

typedef class ViewerPlayer : public Object
{
private :
	glm::mat4 pModel; // player ��
	CFileIn in;

	void drawP(GLuint shaderProgramID);

public :
	ViewerPlayer() : Object() {}

	void init() override;

	// �������� ���� ��� ��ü ��ġ , �� , ũ�� ������ ������Ʈ �ϴ� �Լ�
	void updateViewerPlayer(float newPosX, 
		const glm::vec3& newColor,
		const glm::vec3& newScale);

	void initBuf();
	void render(GLuint shaderProgramID) override;

	void setPosX(float newPosX);

} ViewerPlayer;

