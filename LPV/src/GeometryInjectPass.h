#pragma once
#include<GLEW/glew.h>
#include <GLFW/glfw3.h>
#include"render/RenderManager.h"
#include"render/RenderPass.h"
#include"scene/scene.h"
#include "opengl/Texture.h"
#include"utils.h"
#include<iostream>
class GeometryInjectPass :public RenderPass
{
public:
	std::shared_ptr<Texture> GeometryInjection;
	std::shared_ptr<Scene> scene;
	glm::vec3 Dimensions;
	int rsmResolution;
	int m_FBO = -1;
	int m_VPLVAO = -1;


	GeometryInjectPass(std::string name, const char* vspath, const char* fspath) :RenderPass(name, vspath, fspath) {};


	void drawVPLVAO();
	virtual void init();
	virtual void update();


};

