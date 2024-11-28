#pragma once
#include "object.h"
#include "FileIn.h"

typedef class ViewerPlayer : public Object
{
private :
	glm::mat4 pModel; // player 모델
	CFileIn in;

	bool isRender = true;

	void drawP(GLuint shaderProgramID);

public :
	ViewerPlayer() : Object() {}

	void init() override;

	// 서버에서 받은 뷰어 객체 위치 , 색 , 크기 정보를 업데이트 하는 함수
	void updateViewerPlayerPos(float newPosX);
	void updateViewerPlayerColor(const vec3 newColor);
	void updateViewerPlayerScale(const vec3 newScale);


	void initBuf();
	void render(GLuint shaderProgramID) override;

} ViewerPlayer;

