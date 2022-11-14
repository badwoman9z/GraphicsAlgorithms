#pragma once
#include<GLEW/glew.h>
#include <GLFW/glfw3.h>
#include"render/RenderManager.h"
#include"render/RenderPass.h"
#include"scene/scene.h"
#include "opengl/Texture.h"
#include"utils.h"
#include<iostream>
class FrontWorldPosPass :public RenderPass
{
public:
	FrontWorldPosPass(std::string name, const char* vspath, const char* fspath, const char* gspath = nullptr) :RenderPass(name, vspath, fspath, gspath) {};

	std::shared_ptr<Scene> scene;
	std::shared_ptr<Texture> frontFace;
	unsigned int FBO;
	//unsigned int backFBO;
	GLuint cubeVAO = 0;
	GLuint cubeVBO = 0;
	unsigned int  cubeIndices = 0;
	int VoxelResolution;
	virtual void init();
	virtual void update();

	void RenderCube();

};