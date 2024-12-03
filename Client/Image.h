#pragma once
#include "object.h"


class CImage : public Object
{
public:
	GLint object;
	vector<vec3> vert;
	vector<vec3> norm;
	vector<vec2> uv;

	GLuint texId=0;
	int status{ 0 };

public:
	map<int, int> _map;
	CImage();
	void Bind(int id);
	void render(GLuint);
	void initTex();
	void initBuf();
};

