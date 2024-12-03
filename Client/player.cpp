#include "pch.h"
#include "player.h"
#define CUBE_INDEX_COUNT 36

void Player::init()
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

void Player::clear()
{
	setPos(vec3(0, 0, 0));
	setColor(vec3(1, 0, 0));
	setScale(vec3(1, 1, 1));
	crashOnce = false;
}

void Player::initBuf()
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


void Player::render(GLuint shaderProgramID)
{
	glBindVertexArray(vao);

	model = glm::mat4(1.f);
	model = glm::translate(model, pos);
	model = glm::scale(model, scale);

	drawP(shaderProgramID);
}

void Player::drawP(GLuint shaderProgramID)
{
	glUniform3f(glGetUniformLocation(shaderProgramID, "fColor"), color.r, color.g, color.b);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));

	if (crashOnce) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	if (crashOnce)
		glDisable(GL_BLEND);
}

void Player::moveLeft()
{

	if (pos.x > -0.35f)
	{
		
		pos.x -= 0.05f;
		TextPos -= 0.065f;
	}
}

void Player::moveRight()
{
	if (pos.x < 0.35f)
	{
		pos.x += 0.05f;
		TextPos += 0.065f;
	}
}

void Player::changeRed()
{
	color.r = 1.0f;
	color.g = 0.0f;
	color.b = 0.0f;
}

void Player::changeGreen()
{
	color.r = 0.0f;
	color.g = 1.0f;
	color.b = 0.0f;
}

void Player::changeBlue()
{
	color.r = 0.0f;
	color.g = 0.0f;
	color.b = 1.0f;
}

void Player::changeSize()
{
	if (scale.x == 0.5f)
	{
		scale.x = 1.0f;
		scale.y = 1.0f;
	}
	else
	{
		scale.x = 0.5f;
		scale.y = 0.5f;
	}
}


