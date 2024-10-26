#include "stb_image.h"
#include "base.h"

void Base::init()
{
	const vector<float> temp = {

	   -.5f, 0.f, -1.f, 0, 1, 0, 0, 1,//1
		.4f, 0.f, -1.f, 0, 1, 0, 0, 0,//2
		.4f, 0.f, 1.f, 0, 1, 0, 1, 0,//3
	   -.5f, 0.f, -1.f, 0, 1, 0, 0,1//1

	   ,
	  .4f, 0.f, 1.f, 0, 1, 0, 1, 0,//3
	  -.5f , 0, 1.f, 0, 1, 0, 1, 1//4

	};
	const vector<float> baseColor = {
		0.f, 1.f, 0.f,
		1.f, 0.f, 0.f,
		0.f, 0.f, 1.f,
		1.f, .5f, 1.f };
	const vector<GLubyte> baseIndices = {
		0, 1, 2,
		0, 2, 3 };

	for (int i{}; i < temp.size(); i += 8)
	{
		baseVertices.push_back(glm::vec3(temp[i], temp[i + 1], temp[i + 2]));
		vertices.push_back(temp[i]);
		normals.push_back(glm::vec3(temp[i + 3], temp[i + 4], temp[i + 5]));
		uvs.push_back(glm::vec2(temp[i + 6], temp[i + 7]));
	}

	// ¹Ù´Ú »ö
	color = glm::vec3(0.5f, 0.5f, 0.5f);

	initModel(vertices, baseColor, baseIndices);
	initPos();
	initBuffer();
}

void Base::initBuffer()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, baseVertices.size() * sizeof(glm::vec3), &baseVertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &normBo);
	glBindBuffer(GL_ARRAY_BUFFER, normBo);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &texBo);
	glBindBuffer(GL_ARRAY_BUFFER, texBo);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);
}

void Base::initTexture()
{
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data;

	glBindTexture(GL_TEXTURE_2D, textureID);
	data = stbi_load("texture/floor.png", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
}

void Base::render(GLuint shaderProgramID)
{
	model = glm::mat4(1.f);

	glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(vao);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

