#pragma once
#include<GLEW/glew.h>
#include <GLFW/glfw3.h>
#include"render/RenderManager.h"
#include"render/RenderPass.h"
#include"scene/scene.h"
#include "opengl/Texture.h"
#include"utils.h"
#include<iostream>
class PropagationPass :public RenderPass
{
public:
	std::shared_ptr<Scene> scene;
	glm::ivec3 Dimensions;
	unsigned int m_FBO;
	int m_VPLVAO = -1;
	int m_ProgationNum = 8;
	std::vector<std::shared_ptr<Texture>> LPVRPipeLine;
	std::vector<std::shared_ptr<Texture>> LPVGPipeLine;
	std::vector<std::shared_ptr<Texture>> LPVBPipeLine;

	std::shared_ptr<Texture> LPVAccumulatorR;
	std::shared_ptr<Texture> LPVAccumulatorG;
	std::shared_ptr<Texture> LPVAccumulatorB;

	PropagationPass(std::string name, const char* vspath, const char* fspath, const char* geometryPath = nullptr) :RenderPass(name, vspath, fspath, geometryPath) {};
	void drawVPLVAO();
	virtual void init();
	virtual void update();


};

