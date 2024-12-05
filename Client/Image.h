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

public:
	int GetStatus() { return status; }
	void ChangeScene(int scene);
	map<int, int> _map;
	CImage();
	void Bind(int id);
	void render(GLuint);
	void initTex();
	void initBuf();
private:
	int status{ 0 };
};

