#pragma once
#include "stdafx.h"
#include "object.h"
#include "FileIn.h"

class CMap :public Object
{
public:

	CFileIn in;
	vector<glm::vec2> uvs;
	GLuint texId;

public:
	void drawS(GLuint shaderProgramID);
	void init() override;
	void render(GLuint shaderProgramID) override;

	void initBuf();
	void initTexture();
};

