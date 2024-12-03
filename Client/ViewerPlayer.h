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
	bool isRender = true;
	ViewerPlayer() : Object() {}

	void init() override;

	// �������� ���� ��� ��ü ��ġ , �� , ũ�� ������ ������Ʈ �ϴ� �Լ�
	void updateViewerPlayerPos(float newPosX);
	void updateViewerPlayerColor(const vec3 newColor);
	void updateViewerPlayerScale(const vec3 newScale);


	void initBuf();
	void render(GLuint shaderProgramID) override;

} ViewerPlayer;

