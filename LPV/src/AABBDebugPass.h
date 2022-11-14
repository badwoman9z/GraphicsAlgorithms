#pragma once
#include<GLEW/glew.h>
#include <GLFW/glfw3.h>
#include"render/RenderManager.h"
#include"render/RenderPass.h"
#include"scene/scene.h"
#include "opengl/Texture.h"
#include"utils.h"
#include<iostream>


class AABBDebugPass : public RenderPass
{
public:
	AABBDebugPass(std::string name, const char* vspath, const char* fspath, const char* geometryPath = nullptr) :RenderPass(name, vspath, fspath, geometryPath) {};
	

	virtual void init() override;
	virtual void update() override;
	void drawVPLVAO();

private:
	std::shared_ptr<Scene> scene;
	glm::vec3 m_Min;
	glm::vec3 m_Max;
	glm::ivec3 Dimensions;
	int m_VPLVAO = -1;
	int rsmResolution;
};