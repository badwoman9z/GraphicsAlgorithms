#pragma once
#include<iostream>
#include"render\RenderPass.h"
#include"render\RenderManager.h"
#include"Terrain.h"
#include"scene\scene.h"
class SampleTerrainPass:public RenderPass
{
public:
	SampleTerrainPass(std::string name, const char* vspath, const char* fspath, const char* tessControlPath,
		const char* tessEvalPath) : RenderPass(name, vspath, fspath, tessControlPath, tessEvalPath) {
		terrain = std::make_shared<Terrain>(2,"map/iceland_heightmap.png");
	};
	SampleTerrainPass(std::string name, const char* vspath, const char* fspath, const char* geometryPath = nullptr) : RenderPass(name, vspath, fspath, geometryPath) {
		terrain = std::make_shared<Terrain>(2, "map/iceland_heightmap.png");
	};
	virtual void init();
	virtual void update();
	std::shared_ptr<Scene> scene;
private:
	std::shared_ptr<Terrain> terrain;

};

