#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Image.h"

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

void CImage::render(GLuint ID)
{
	glUseProgram(ID);

	model = mat4(1.0);
	model = translate(model, pos);
	model = glm::scale(model, scale);

	glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(vao);

	glBindTexture(GL_TEXTURE_2D, texId);
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
	glGenTextures(1, &texId);

	glBindTexture(GL_TEXTURE_2D, texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, channel;
	unsigned char* data;

	glBindTexture(GL_TEXTURE_2D, texId);
	//stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�

	if (0 == status)
		data = stbi_load("texture/start.jpg", &width, &height, &channel, 0);
	else if (1 == status)
		data = stbi_load("texture/b.png", &width, &height, &channel, 0);
	else if (2 == status)
		data = stbi_load("texture/win.jpg", &width, &height, &channel, 0);
	else if (3 == status)
		data = stbi_load("texture/over.jpg", &width, &height, &channel, 0);
	else if (4 == status)
		data = stbi_load("texture/b2.png", &width, &height, &channel, 0);
	else
		data = stbi_load("texture/b3.png", &width, &height, &channel, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}
