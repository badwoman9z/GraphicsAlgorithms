#pragma once
#include <iostream>
#include<vector>
#include"scene\vertex.h"
#include<GLEW/glew.h>
#include <GLFW/glfw3.h>
#include"render\shader.h"
#include"scene\vertex.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Terrain
{
public:
	Terrain(int type, const char* path);
	void Draw(Shader shader);
	void DrawElements(Shader shader);
	void LoadTerrianTexture(const char* path);
	void LoadTerrian(const char* path);
	
	
	
	void setUpNormals();
	void setUpIndices();
	void setUpPositionsAndUV(const char* path);
	void buildUpVAO();
private:
	int width;
	int height;
	int nChannels;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	unsigned int rez = 20;
	unsigned int heightTexture;
	std::vector<float> vertices;
	std::vector<Varying> m_vertices;
	std::vector<unsigned> indices;
	unsigned int NUM_STRIPS;
	unsigned int NUM_VERTS_PER_STRIP;
};
