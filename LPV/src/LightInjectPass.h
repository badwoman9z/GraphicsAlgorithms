#pragma once
#include<GLEW/glew.h>
#include <GLFW/glfw3.h>
#include"render/RenderManager.h"
#include"render/RenderPass.h"
#include"scene/scene.h"
#include "opengl/Texture.h"
#include"utils.h"
#include<iostream>
class LightInjectPass : public RenderPass {
public :
	std::shared_ptr<Texture> LPVGridR;
	std::shared_ptr<Texture> LPVGridG;
	std::shared_ptr<Texture> LPVGridB;
	std::shared_ptr<Scene> scene;
	glm::ivec3 Dimensions;
	int rsmResolution;
	unsigned m_FBO ;
	int m_VPLVAO = -1;


	LightInjectPass(std::string name, const char* vspath, const char* fspath, const char* geometryPath = nullptr) :RenderPass(name, vspath, fspath,geometryPath) {};


	void drawVPLVAO();
	virtual void init();
	virtual void update();
};