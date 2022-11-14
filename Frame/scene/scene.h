#pragma once
#include<iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include<vector>
#include"camera.h"
#include"Light.h"
#include"object.h"
#include"render/shader.h"
#include"AABB.h"
#include"Model.h"
class Scene
{
public:

	glm::mat4 view;
	glm::mat4 projection;

	std::shared_ptr<Camera> camera;

	Light light;
	int SCR_WIDTH = 1920;
	int SCR_HEIGHT = 1080;
	int RSMResolution;
	int VoxelResolution;
	float nearPlane = 0.1f;
	float farPlane = 60.0f;
	std::shared_ptr<AABB>     bounding;


	std::vector<std::shared_ptr<Model>> models;

	Scene(Camera c, Light l,float far) :camera(std::make_shared<Camera>(c)), light(l),farPlane(far) {
		view = camera->GetViewMatrix();
		projection = glm::perspective(glm::radians(camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, nearPlane, farPlane);

	}

	void LoadModel(const std::string& pfile);
	void LoadModel(const std::string& pfile,int num);
	std::shared_ptr<AABB> getOrCreateBounding() {
		if (!bounding) {
			bounding = std::make_shared<AABB>();

			for (int i = 0; i < models.size(); i++) {
				bounding->combine(*(models[i]->getOrCreateAABB()));
			}
		}
		return bounding;
	}
	void Draw(Shader shader);
	void DrawInstance(Shader shader);
	void Update();
	void Init();

};

