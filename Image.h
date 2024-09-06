#pragma once
#include "object.h"

class CImage : public Object
{
public:
	GLint object;
	vector<vec3> vert;
	vector<vec3> norm;
	vector<vec2> uv;

	GLuint texId;
	int status{ 0 };

public:
	CImage();
	void render(GLuint);
	void initTex();
	void initBuf();
};

