#pragma once
#include "object.h"

typedef class Base : public Object
{
	GLuint textureID;
	vector<glm::vec3> normals;
	vector<glm::vec2> uvs;
	vector<glm::vec3> baseVertices;

public:
	void init() override;
	void initTexture();
	void initBuffer();
	void render(GLuint shaderProgramID) override;
} Base;

