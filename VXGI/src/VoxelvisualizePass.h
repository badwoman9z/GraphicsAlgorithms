#pragma once
#include<GLEW/glew.h>
#include <GLFW/glfw3.h>
#include"render/RenderManager.h"
#include"render/RenderPass.h"
#include"scene/scene.h"
#include "opengl/Texture.h"
#include"utils.h"
#include<iostream>
class VoxelvisualizePass :public RenderPass
{
public:
	VoxelvisualizePass(std::string name, const char* vspath, const char* fspath, const char* gspath = nullptr) :RenderPass(name, vspath, fspath, gspath) {};

	GLuint cubeVAO = 0;
	GLuint cubeVBO = 0;
	unsigned int  cubeIndices = 0;
	int VoxelResolution;
	virtual void init();
	virtual void update();

	void RenderCube();

};