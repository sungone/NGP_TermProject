#pragma once

class CFileIn
{
public:

	std::vector< GLubyte> vertexIndices, uvIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec3> temp_normals;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec2> out_uvs;

	std::vector<glm::vec3> out_vertices, out_normals;

public:
	int readObj(const char* fileName);
};

