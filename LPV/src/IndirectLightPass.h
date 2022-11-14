#pragma once
#include<GLEW/glew.h>
#include <GLFW/glfw3.h>
#include"render/RenderManager.h"
#include"render/RenderPass.h"
#include"scene/scene.h"
#include<iostream>
#include "opengl/Texture.h"
class IndirectLightPass :public RenderPass
{
public:
	IndirectLightPass(std::string name, const char* vspath, const char* fspath) :RenderPass(name, vspath, fspath) {};
	std::shared_ptr<Scene> scene;
	std::shared_ptr<Texture> indirectLight;
	unsigned int FBO;
	virtual void init();
	virtual void update();


};
