#include "wall.h"
#define CUBE_INDEX_COUNT 36

// ���� �Լ�
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> dis(0, 99);
std::uniform_int_distribution<int> dis2(0, 1);



void Cube::init()
{
	const vector<float> CubeVertices = {

		-0.5f, 0.f, -1.f,
		-0.2f, 0.f, -1.f,
		-0.5f , 0.3f , -1.f,
		-0.2f , 0.3f , -1.f,

		 -0.5f, 0.f, -0.7f,
		-0.2f, 0.f, -.7f,
		-0.5f , 0.3f , -.7f,
		-0.2f , 0.3f , -.7f,
	};

	vector<float> CubeColors;
	for (int i = 0; i < 8; i++)
	{
		CubeColors.push_back(1.f);
		CubeColors.push_back(0.f);
		CubeColors.push_back(0.f);
	}

	const vector<GLubyte> CubeIndices = {

		// �ո�
		4 , 5 , 7,
		7 , 6 , 4,
		// �޸�
		1 , 0 , 3,
		0 , 2 , 3,
		// �ٴڸ�
		1 , 5 , 4,
		4 , 0 , 1,
		// ����
		3 , 2 , 6,
		6 , 7 , 3,
		// �����ʸ�
		1 , 3 , 7,
		7 , 5 , 1,
		// ���ʸ�
		6 , 2 , 0,
		0 , 4 , 6,
	};

	initModel(CubeVertices, CubeColors, CubeIndices);
	color.r = 1.0f;
	color.g = 0.0f;
	color.b = 0.0f;
	initBuffer();
}

void Cube::render(GLuint shaderProgramID)
{
	glBindVertexArray(vao);

	model = glm::mat4(1.f);
	model = glm::translate(model, pos);
	model = glm::scale(model, scale);

	drawCube(shaderProgramID);
}

void Cube::reset()
{
	initPos();
	color = vec3(1.0f, 0.0f, 0.0f);
}

bool Cube::moveCube()
{
	if (pos.z >= 1.7f)
	{
		reset();
		return true;
	}

	pos.z += 0.01f;
	return false;
}

void Cube::changeEmpty()
{
	pos.x += 10.0f;
}

void Cube::changeRed()
{
	color.r = 1.0f;
	color.g = 0.0f;
	color.b = 0.0f;
}

void Cube::changeGreen()
{
	color.r = 0.0f;
	color.g = 1.0f;
	color.b = 0.0f;
}

void Cube::changeBlue()
{
	color.r = 0.0f;
	color.g = 0.0f;
	color.b = 1.0f;
}

void Cube::change_SmallSize()
{
	pos.y = 0.2f;
	scale.y = 0.4f;
}

void Cube::drawCube(GLuint shaderProgramID)
{
	cModel = glm::mat4(1.f);
	cModel = glm::translate(cModel, glm::vec3(0.f, 0.f, 0.f));
	cModel = glm::scale(cModel, vec3(1.0f, 1.0f, 1.0f));
	model = model * cModel;

	glUniform3f(glGetUniformLocation(shaderProgramID, "fColor"), color.r, color.g, color.b);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawElements(GL_TRIANGLES, CUBE_INDEX_COUNT, GL_UNSIGNED_BYTE, 0);
}

void Wall::init()
{
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			cube[i][j].init();
			cube[i][j].setPosX(0.3f * j);
			cube[i][j].setPosY(0.3f * i);
		}
	}

	for (int i = 0; i < 36; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			random_num[i][j] = dis(gen);
		}
	}
}

void Wall::render(GLuint shaderProgramID)
{
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			cube[i][j].render(shaderProgramID);
		}
	}
}

Cube Wall::getCube(int i, int j)
{
	return cube[i][j];
}

void Wall::moveWall()
{
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if (cube[i][j].moveCube())
			{
				cube[i][j].setPosX(0.3f * j);
				cube[i][j].setPosY(0.3f * i);

				makeWall(i, j, cur_idx);

				if (i == 2 && j == 2)
				{
					++cur_idx;
				}
			}
		}
	}
}

void Wall::makeWall(int i, int j, int cur_idx)
{

	if (cur_idx >= 0 && cur_idx <= 9) // cur_idx 0 ~ 9 1�ܰ�
	{
		if (random_num[i + cur_idx][j] % 2 == 0)
		{
			cube[i][j].changeEmpty();


		}
		else if (0 == i) {
			if (2 == j) {
				if (0 != random_num[i + cur_idx][0] % 2 and 0 != random_num[i + cur_idx][1] % 2) {
					int empty = dis2(gen);
					cube[0][empty].changeEmpty();


					emptyIdx.erase(emptyIdx.begin() + empty);
					emptyIdx.push_back(vec2(0, 2));

				}
				else
					emptyIdx.push_back(vec2(0, 2));
			}
			else
				emptyIdx.push_back(vec2(0, j));
		}
	}
	else if (cur_idx >= 10 && cur_idx <= 19) // cur_idx 10 ~ 19 2�ܰ� - rgb
	{
		if (random_num[i + cur_idx][j] % 3 == 0)
		{
			cube[i][j].changeRed();

			if (0 == i)
				emptyIdx.push_back(vec2(0, j));
		}
		else if (random_num[i + cur_idx][j] % 3 == 1)
		{
			cube[i][j].changeGreen();

			if (0 == i)
				emptyIdx.push_back(vec2(1, j));
		}
		else if (random_num[i + cur_idx][j] % 3 == 2)
		{
			cube[i][j].changeBlue();

			if (0 == i)
				emptyIdx.push_back(vec2(2, j));
		}

	}
	else if (cur_idx >= 20 && cur_idx <= 29) // cur_idx 20 ~ 29 3�ܰ� - size
	{
		if (random_num[i + cur_idx][j] % 2 == 0)
		{
			cube[i][j].change_SmallSize();

			if (0 == i)
				emptyIdx.push_back(vec2(0, j));
		}
		else if (0 == i) {

			if (2 == j) {
				if (0 != random_num[i + cur_idx][0] % 2 and 0 != random_num[i + cur_idx][1] % 2) {
					int empty = dis2(gen);
					cube[0][empty].change_SmallSize();


					emptyIdx.erase(emptyIdx.begin() + empty);
					emptyIdx.push_back(vec2(1, 2));

				}
				else
					emptyIdx.push_back(vec2(1, 2));
			}
			else
				emptyIdx.push_back(vec2(1, j));

		}
	}
}

