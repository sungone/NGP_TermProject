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

	// �������� ���� ��� ��ü ��ġ , �� , ũ�� ������ ������Ʈ �ϴ� �Լ�
	void updateViewerPlayer(const glm::vec3& newPos, 
		const glm::vec3& newColor,
		const glm::vec3& newScale);

	void render(GLuint shaderProgramID) override;

} ViewerPlayer;

