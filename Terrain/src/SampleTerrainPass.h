#pragma once
#include<iostream>
#include"render\RenderPass.h"
#include"Terrain.h"
class SampleTerrainPass:public RenderPass
{
public:
	SampleTerrainPass(std::string name, const char* vspath, const char* fspath, const char* gspath = nullptr) : RenderPass(name, vspath, fspath, gspath) {
		terrain = std::make_shared<Terrain>("map/iceland_heightmap.png");
	};
	virtual void init();
	virtual void update();
private:
	std::shared_ptr<Terrain> terrain;

};

