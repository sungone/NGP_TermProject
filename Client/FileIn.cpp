#include "pch.h"
#include "FileIn.h"

int CFileIn::readObj(const char* fileName)
{
	FILE* objFile;
	vector<vector<float>> tmp;
	vector<int> id;

	errno_t err = fopen_s(&objFile, fileName, "rb");

	if (NULL == objFile) {
		cout << "error" << endl;
		printf("Error opening file. Error code: %d\n", err);
		return false;
	}

	while (true) {

		char lineHeader[65535];

		int res = fscanf(objFile, "%s", lineHeader);
		if (EOF == res) 
			break;
		

		if (not strcmp(lineHeader, "v")) {
			glm::vec3 vertex;
			fscanf(objFile, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

			if (strcmp(fileName, "object/Scene.obj")) {
				vertex.x /= 200.f;
				vertex.y /= 200.f;
				vertex.z /= 200.f;
				vertex.z += 0.8f;
			}
			else {
				vertex.x /= 800.f;
				vertex.y /= 800.f;
				vertex.z /= 800.f;
				vertex.y -= 0.55f;
				vertex.x += 2.29f;
			}

			temp_vertices.push_back(vertex);
		}
		else if (not strcmp(lineHeader, "vn")) {
			glm::vec3 normal;
			fscanf(objFile, "%f %f %f\n", &normal.x, &normal.y, &normal.z);


			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			glm::vec2 uv;
			fscanf(objFile, "%f %f\n", &uv.x, &uv.y);

			temp_uvs.push_back(uv);
		}

		else if (not strcmp(lineHeader, "f")) {

			float vertexIndex[4];
			float uvIndex[4];
			vector<float>t;

			int matches;

			if (strcmp(fileName, "object/Scene.obj"))
				matches = fscanf(objFile, "%f//%*f %f//%*f %f//%*f %f//%*f", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2], &vertexIndex[3]);
			else
				matches = fscanf(objFile, "%f/%f/%*f %f/%f/%*f %f/%f/%*f %f/%f/%*f", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2], &vertexIndex[3], &uvIndex[3]);

			if (6 == matches or 3 == matches) {

				vertexIndices.push_back(vertexIndex[0] - 1);
				vertexIndices.push_back(vertexIndex[1] - 1);
				vertexIndices.push_back(vertexIndex[2] - 1);

				if (6 == matches) {
					uvIndices.push_back(uvIndex[0] - 1);
					uvIndices.push_back(uvIndex[1] - 1);
					uvIndices.push_back(uvIndex[2] - 1);
				}


				t.push_back(vertexIndex[0] - 1);
				t.push_back(vertexIndex[1] - 1);
				t.push_back(vertexIndex[2] - 1);
				id.push_back(3);

			}
			else if (8 == matches or 4 == matches) {
				// 사각형의 경우
				vertexIndices.push_back(vertexIndex[0] - 1);
				vertexIndices.push_back(vertexIndex[1] - 1);
				vertexIndices.push_back(vertexIndex[2] - 1);

				vertexIndices.push_back(vertexIndex[0] - 1);
				vertexIndices.push_back(vertexIndex[2] - 1);
				vertexIndices.push_back(vertexIndex[3] - 1);


				t.push_back(vertexIndex[0] - 1);
				t.push_back(vertexIndex[1] - 1);
				t.push_back(vertexIndex[2] - 1);

				t.push_back(vertexIndex[0] - 1);
				t.push_back(vertexIndex[2] - 1);
				t.push_back(vertexIndex[3] - 1);
				id.push_back(6);
			}
			tmp.push_back(t);

		}

	}

	for (int i{}; i < tmp.size(); ++i) {
		for (int j{}; j < id[i]; ++j) {
			out_vertices.push_back(temp_vertices[tmp[i][j]]);
			out_normals.push_back(temp_normals[tmp[i][j]]);
		}
	}

	if (not temp_uvs.empty()) {
		for (unsigned int i{}; i < uvIndices.size(); ++i)
		{
			unsigned int uvIndex{ uvIndices[i] };
			glm::vec2 uv{ temp_uvs[uvIndex] };
			out_uvs.push_back(uv);
		}
	}
}





