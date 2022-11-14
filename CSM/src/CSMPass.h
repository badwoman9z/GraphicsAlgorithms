#pragma once
#include<GLEW/glew.h>
#include <GLFW/glfw3.h>
#include"render/RenderManager.h"
#include"render/RenderPass.h"
#include"scene/scene.h"
#include "opengl/Texture.h"
#include<iostream>
struct Frustum {
	float Near;
	float Far;
};

class CSMPass:public RenderPass
{
public:



	CSMPass(std::string name, const char* vspath, const char* fspath, const char* gspath = nullptr) : RenderPass(name, vspath, fspath, gspath) {
		scene = RenderManager::get_instance().getScene();
		float n = scene->nearPlane;
		float f = scene->farPlane;
		frustum.resize(4);
		frustum[0].Near = n;
		for (int i = 1; i < 4; i++) {
			float s = float(i) / 4.0f;
			
			float t_z = n + (f-n)*s;
			float log_z = n * pow((f / n), s);

			frustum[i].Near = 0.6*t_z+0.4*log_z;
			frustum[i - 1].Far = 0.6 * t_z + 0.4 * log_z;
			
		}
		frustum[3].Far = f;

		for (int i = 0; i < 4; i++) {
			scene->light.frustumFarPlane.push_back(frustum[i].Far);
		}

		frustumProjectionMat.resize(4);

	};


	std::vector<Frustum> frustum;
	std::vector<glm::mat4> frustumProjectionMat;
	std::shared_ptr<Scene> scene;
	std::shared_ptr<Texture> depthTexture;
	std::shared_ptr<Texture>  depthTexture2;
	float resulotion = 1024.f;
	unsigned int FBO;
	
	virtual void init();

	virtual void update();


	void updateCompactFrustum();

	void updateOuterSphereFrustum();

};

