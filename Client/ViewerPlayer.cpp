#include "pch.h"
#include "ViewerPlayer.h"
#define CUBE_INDEX_COUNT 36

//void ViewerPlayer::drawP(GLuint shaderProgramID)
//{
//	pModel = glm::mat4(1.f);
//	pModel = glm::translate(pModel, glm::vec3(0.f, 0.f, 0.f));
//	pModel = glm::scale(pModel, glm::vec3(scale.x, scale.y, scale.z));
//	model = model * pModel;
//
//	glUniform3f(glGetUniformLocation(shaderProgramID, "fColor"), color.r, color.g, color.b);
//	glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));
//	glDrawElements(GL_TRIANGLES, CUBE_INDEX_COUNT, GL_UNSIGNED_BYTE, 0);
//}

//void ViewerPlayer::init()
//{
//	const vector<float> playerVertices = {
//		-0.1f , 0.0f , 0.8f,
//		0.0f , 0.0f , 0.8f,
//		-0.1f , 0.1f , 0.8f,
//		0.0f , 0.1f , 0.8f,
//
//		-0.1f , 0.0f , 0.7f,
//		0.0f , 0.0f , 0.7f,
//		-0.1f , 0.1f , 0.7f,
//		0.0f , 0.1f , 0.7f,
//	};
//
//	vector<float> playerColors;
//
//	for (int i = 0; i < 8; i++)
//	{
//		playerColors.push_back(1.f);
//		playerColors.push_back(0.f);
//		playerColors.push_back(0.f);
//	}
//
//	const vector<GLubyte> playerIndices = {
//		// ¾Õ¸é
//		4 , 5 , 7,
//		7 , 6 , 4,
//		// µÞ¸é
//		1 , 0 , 3,
//		0 , 2 , 3,
//		// ¹Ù´Ú¸é
//		1 , 5 , 4,
//		4 , 0 , 1,
//		// À­¸é
//		3 , 2 , 6,
//		6 , 7 , 3,
//		// ¿À¸¥ÂÊ¸é
//		1 , 3 , 7,
//		7 , 5 , 1,
//		// ¿ÞÂÊ¸é
//		6 , 2 , 0,
//		0 , 4 , 6,
//	};
//
//	color = glm::vec3(1.0f, 0.0f, 0.0f);
//
//	initModel(playerVertices, playerColors, playerIndices);
//	initBuffer();
//}

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
		// ¾Õ¸é
		4 , 5 , 7,
		7 , 6 , 4,
		// µÞ¸é
		1 , 0 , 3,
		0 , 2 , 3,
		// ¹Ù´Ú¸é
		1 , 5 , 4,
		4 , 0 , 1,
		// À­¸é
		3 , 2 , 6,
		6 , 7 , 3,
		// ¿À¸¥ÂÊ¸é
		1 , 3 , 7,
		7 , 5 , 1,
		// ¿ÞÂÊ¸é
		6 , 2 , 0,
		0 , 4 , 6,
	};

	color = glm::vec3(1.0f, 0.0f, 0.0f);

	vector<float> pv;
	vector< GLubyte>pi;

	in.readObj("object/voodoo5.obj");

	for (int i{}; i < in.out_vertices.size(); ++i) {
		pv.push_back(in.out_vertices[i].x);
		pv.push_back(in.out_vertices[i].y);
		pv.push_back(in.out_vertices[i].z);
	}

	for (int i{}; i < in.out_normals.size(); ++i)
		normals.push_back(in.out_normals[i]);

	for (int i{}; i < in.vertexIndices.size(); ++i)
		pi.push_back(in.vertexIndices[i]);

	initModel(pv, playerColors, pi);
	initBuf();
}

void ViewerPlayer::drawP(GLuint shaderProgramID)
{
	glUniform3f(glGetUniformLocation(shaderProgramID, "fColor"), color.r, color.g, color.b);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

void ViewerPlayer::clear()
{
	setPos(vec3(0, 0, 0));
	setColor(vec3(1, 0, 0));
	setScale(vec3(1, 1, 1));
	isRender = true;
}

void ViewerPlayer::initBuf()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &normBo);
	glBindBuffer(GL_ARRAY_BUFFER, normBo);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
}

void ViewerPlayer::updateViewerPlayerPos(float newPosX)
{
	pos.x = newPosX;
}

void ViewerPlayer::updateViewerPlayerColor(const vec3 newColor)
{
	color.r = newColor.r;
	color.g = newColor.g;
	color.b = newColor.b;
}

void ViewerPlayer::updateViewerPlayerScale(const vec3 newScale)
{
	scale.x = newScale.x;
	scale.y = newScale.y;
}

void ViewerPlayer::render(GLuint shaderProgramID)
{
	if (isRender)
	{
		glBindVertexArray(vao);

		model = glm::mat4(1.f);
		model = glm::translate(model, pos);
		model = glm::scale(model, scale);

		drawP(shaderProgramID);
	}
}
