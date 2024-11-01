#include "pch.h"
#include "ViewerPlayer.h"
#define CUBE_INDEX_COUNT 36

void ViewerPlayer::drawP(GLuint shaderProgramID)
{
	pModel = glm::mat4(1.f);
	pModel = glm::translate(pModel, glm::vec3(0.f, 0.f, 0.f));
	pModel = glm::scale(pModel, glm::vec3(scale.x, scale.y, scale.z));
	model = model * pModel;

	glUniform3f(glGetUniformLocation(shaderProgramID, "fColor"), color.r, color.g, color.b);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawElements(GL_TRIANGLES, CUBE_INDEX_COUNT, GL_UNSIGNED_BYTE, 0);
}

void ViewerPlayer::init()
{
	const vector<float> playerVertices = {
		-0.1f , 0.0f , 0.8f,
		0.0f , 0.0f , 0.8f,
		-0.1f , 0.1f , 0.8f,
		0.0f , 0.1f , 0.8f,

		-0.1f , 0.0f , 0.7f,
		0.0f , 0.0f , 0.7f,
		-0.1f , 0.1f , 0.7f,
		0.0f , 0.1f , 0.7f,
	};

	vector<float> playerColors;

	for (int i = 0; i < 8; i++)
	{
		playerColors.push_back(1.f);
		playerColors.push_back(0.f);
		playerColors.push_back(0.f);
	}

	const vector<GLubyte> playerIndices = {
		// 앞면
		4 , 5 , 7,
		7 , 6 , 4,
		// 뒷면
		1 , 0 , 3,
		0 , 2 , 3,
		// 바닥면
		1 , 5 , 4,
		4 , 0 , 1,
		// 윗면
		3 , 2 , 6,
		6 , 7 , 3,
		// 오른쪽면
		1 , 3 , 7,
		7 , 5 , 1,
		// 왼쪽면
		6 , 2 , 0,
		0 , 4 , 6,
	};

	color = glm::vec3(1.0f, 0.0f, 0.0f);

	// To Do : Obj 파일 집어넣어서 캐릭터 만들기
	//vector<float> pv;
	//vector< GLubyte>pi;

	//in.readObj("object/voodoo5.obj");

	//for (int i{}; i < in.out_vertices.size(); ++i) {
	//	pv.push_back(in.out_vertices[i].x);
	//	pv.push_back(in.out_vertices[i].y);
	//	pv.push_back(in.out_vertices[i].z);
	//}

	//for (int i{}; i < in.out_normals.size(); ++i)
	//	normals.push_back(in.out_normals[i]);

	//for (int i{}; i < in.vertexIndices.size(); ++i)
	//	pi.push_back(in.vertexIndices[i]);

	//initModel(pv, playerColors, pi);

	initModel(playerVertices, playerColors, playerIndices);
	initBuffer();
}

void ViewerPlayer::updateViewerPlayer(float newPosX, const glm::vec3& newColor, const glm::vec3& newScale)
{
	pos.x = newPosX;
	
	color.r = newColor.r;
	color.g = newColor.g;
	color.b = newColor.b;

	scale.x = newScale.x;
	scale.y = newScale.y;
}

void ViewerPlayer::render(GLuint shaderProgramID)
{
	glBindVertexArray(vao);

	model = glm::mat4(1.f);
	model = glm::translate(model, pos);
	model = glm::scale(model, scale);

	drawP(shaderProgramID);
}

void ViewerPlayer::setPosX(float newPosX)
{
	pos.x = newPosX;
}
