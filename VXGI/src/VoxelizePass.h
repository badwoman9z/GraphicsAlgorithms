#pragma once
#include<GLEW/glew.h>
#include <GLFW/glfw3.h>
#include"render/RenderManager.h"
#include"render/RenderPass.h"
#include"scene/scene.h"
#include "opengl/Texture.h"
#include<iostream>
class VoxelizePass :public RenderPass
{
public:
	VoxelizePass(std::string name, const char* vspath, const char* fspath ,const char* gspath = nullptr) :RenderPass(name, vspath, fspath, gspath) {};

	std::shared_ptr<Scene> scene;
	std::shared_ptr<Texture> voxelTexture;
	glm::vec3 min_AABB;
	glm::vec3 Dimensions;
	glm::mat4 projX;
	glm::mat4 projY;
	glm::mat4 projZ;
	glm::mat4 projectMat;
	int VoxelResolution = 256;
	float VoxelWorldSize = 150.0f;
	int isinit = 0;

	virtual void init();
	virtual void update();


};

