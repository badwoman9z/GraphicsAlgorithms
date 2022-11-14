#pragma once
#include<GLEW/glew.h>
#include <GLFW/glfw3.h>
#include"render/RenderManager.h"
#include"render/RenderPass.h"
#include"scene/scene.h"
#include "opengl/Texture.h"
#include<iostream>
class RSMBufferPass:public RenderPass
{
public:
	unsigned int FBO;
	unsigned int RBO;
	int RSMResolution = 128;
	std::shared_ptr<Scene> scene;
	std::shared_ptr<Texture> rsm_flux;
	std::shared_ptr<Texture> rsm_normal;
	std::shared_ptr<Texture> rsm_position;
	RSMBufferPass(std::string name, const char* vspath, const char* fspath) :RenderPass(name, vspath, fspath) {};
	virtual void init();
	virtual void update();
	
};

