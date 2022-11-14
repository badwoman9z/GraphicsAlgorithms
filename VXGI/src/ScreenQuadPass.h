#pragma once
#include<GLEW/glew.h>
#include <GLFW/glfw3.h>
#include"render/RenderManager.h"
#include"render/RenderPass.h"
#include"scene/scene.h"
#include<iostream>
class ScreenQuadPass:public RenderPass
{
public:
	
	ScreenQuadPass(std::string name, const char* vspath, const char* fspath) :RenderPass(name, vspath, fspath) {};

	virtual void init();
	virtual void update();
};
