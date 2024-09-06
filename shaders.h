#pragma once
#include "stdafx.h"

void makeVertexShaders(char* file);
void makeFragmentShaders(char* file);
GLuint initShader(char* vertexFile, char* fragmentFile);
char* fileToBuf(char* fileName);
