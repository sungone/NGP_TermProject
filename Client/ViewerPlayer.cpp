#include "pch.h"
#include "ViewerPlayer.h"

void ViewerPlayer::drawP(GLuint shaderProgramID)
{
	glUniform3f(glGetUniformLocation(shaderProgramID, "fColor"), color.r, color.g, color.b);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));
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
	color.r = 1.0f;
	color.g = 0.0f;
	color.b = 0.0f;
	initBuf();
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

	glGenBuffers(1, &cbo);
	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), &colors[0], GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	if (!indices.empty())
	{
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), &indices[0], GL_STATIC_DRAW);
	}
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
