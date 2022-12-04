#pragma once
#include <iostream>
#include<vector>
#include"scene\vertex.h"
#include<GLEW/glew.h>
#include <GLFW/glfw3.h>
class Terrain
{
public:
	Terrain(const char* path);
	void Draw();
private:
	int width;
	int height;
	int nChannels;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	std::vector<unsigned int> indices;
	std::vector<float> vertices;
	unsigned int NUM_STRIPS;
	unsigned int NUM_VERTS_PER_STRIP;
};
