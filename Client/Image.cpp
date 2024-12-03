#include "pch.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Image.h"
#include "TextManager.h"

CImage::CImage()
{
	const vector<float> first{ {
		1, 0, -10, 0, 0, 1, 0, 1,
		1, 1, -10, 0, 0, 1, 0, 0,
		0, 1, -10, 0, 0, 1, 1, 0,
		1, 0, -10, 0, 0, 1, 0, 1,
		0, 1, -10, 0, 0, 1, 1, 0,
		0, 0, -10, 0, 0, 1, 1, 1 } };

	for (int i{}; i < first.size(); i += 8) {
		vert.push_back(vec3(first[i], first[i + 1], first[i + 2]));
		norm.push_back(vec3(first[i + 3], first[i + 4], first[i + 5]));
		uv.push_back(vec2(first[i + 6], first[i + 7]));
	}
	initPos();
	pos = vec3(-25, -25, 0);
	scale = vec3(50, 50, 1);
}

void CImage::Bind(int id)
{
	int key = _map[id];
	glBindTexture(GL_TEXTURE_2D, key);
}

void CImage::render(GLuint ID)
{
	glUseProgram(ID);

	model = mat4(1.0);
	model = translate(model, pos);
	model = glm::scale(model, scale);

	glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0, vert.size());

	
}

void CImage::initBuf()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(glm::vec3), &vert[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &normBo);
	glBindBuffer(GL_ARRAY_BUFFER, normBo);
	glBufferData(GL_ARRAY_BUFFER, norm.size() * sizeof(glm::vec3), &norm[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &texBo);
	glBindBuffer(GL_ARRAY_BUFFER, texBo);
	glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(glm::vec2), &uv[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);
}

void CImage::initTex()
{

	for (int i = 0; i <= 6; ++i)
	{
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		int width, height, channel;
		unsigned char* data;
		glBindTexture(GL_TEXTURE_2D, texId);
		//stbi_set_flip_vertically_on_load(true); //--- 이미지가 거꾸로 읽힌다면 추가

		if (0 == i)
			data = stbi_load("texture/start.jpg", &width, &height, &channel, 0);
		else if (1 == i)
			data = stbi_load("texture/b.png", &width, &height, &channel, 0);
		else if (2 == i)
			data = stbi_load("texture/win.jpg", &width, &height, &channel, 0);
		else if (3 == i)
			data = stbi_load("texture/over.jpg", &width, &height, &channel, 0);
		else if (4 == i)
			data = stbi_load("texture/b2.png", &width, &height, &channel, 0);
		else if (5 == i)
			data = stbi_load("texture/b3.png", &width, &height, &channel, 0);
		else  // 게임 매칭 대기창
			data = stbi_load("texture/matching.jpg", &width, &height, &channel, 0);

		int format = (channel == 4) ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		_map[i] = texId;
		stbi_image_free(data);
	}
}
