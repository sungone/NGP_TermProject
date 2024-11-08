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
	void updateViewerPlayerPos(float newPosX);
	void updateViewerPlayerColor(const vec3 newColor);
	void updateViewerPlayerScale(const vec3 newScale);

	void render(GLuint shaderProgramID) override;

} ViewerPlayer;

