#pragma once
#include<iostream>
#include"shader.h"
class RenderPass
{
public:
	
	RenderPass(std::string name, const char* vspath, const char* fspath, const char* geometryPath = nullptr) :passName(name) {
		shader = std::make_shared<Shader>(vspath, fspath, geometryPath);
	}
	RenderPass(std::string name, const char* cspath) :passName(name) {
		shader = std::make_shared<Shader>(cspath);
	}
	RenderPass(std::string name, const char* vspath, const char* fspath, const char* tessControlPath,
		const char* tessEvalPath) :passName(name) {
		shader = std::make_shared<Shader>(vspath, fspath, tessControlPath, tessEvalPath);
	}
	std::string passName;
	std::shared_ptr<Shader> shader;
	virtual void init() = 0;
	virtual void update() = 0;

};

