#pragma once
#include<GLEW/glew.h>
#include <GLFW/glfw3.h>
#include"render/RenderManager.h"
#include"render/RenderPass.h"
#include"scene/scene.h"
#include<iostream>
class CSMScreenQuadPass :public RenderPass
{
public:
	std::shared_ptr<Scene> scene;
	CSMScreenQuadPass(std::string name, const char* vspath, const char* fspath) :RenderPass(name, vspath, fspath) {
		scene = RenderManager::get_instance().getScene();
		
	};

	virtual void init();
	virtual void update();
};
