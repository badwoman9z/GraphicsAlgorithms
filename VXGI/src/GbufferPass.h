#pragma once
#include<GLEW/glew.h>
#include <GLFW/glfw3.h>
#include"render/RenderManager.h"
#include"render/RenderPass.h"
#include"scene/scene.h"
#include "opengl/Texture.h"
#include<iostream>
class GbufferPass:public RenderPass
{
public:
	unsigned int FBO;
	unsigned int RBO;
	std::shared_ptr<Scene> scene;
	std::shared_ptr<Texture> color;
	std::shared_ptr<Texture> normal;
	std::shared_ptr<Texture> position;
	GbufferPass(std::string name, const char* vspath, const char* fspath) :RenderPass(name, vspath, fspath) {};
	
	virtual void init();
	virtual void update();

};

