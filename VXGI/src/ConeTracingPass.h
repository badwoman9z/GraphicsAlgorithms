#pragma once
#include<GLEW/glew.h>
#include <GLFW/glfw3.h>
#include"render/RenderManager.h"
#include"render/RenderPass.h"
#include"scene/scene.h"
#include "opengl/Texture.h"
#include"utils.h"
#include<iostream>
class ConeTracingPass :public RenderPass {


public :
	ConeTracingPass(std::string name, const char* vspath, const char* fspath, const char* gspath = nullptr) :RenderPass(name, vspath, fspath, gspath) {};
	virtual void init();
	virtual void update();

	std::shared_ptr<Scene> scene;

};