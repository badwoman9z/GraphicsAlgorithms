#pragma once
#include"RenderPass.h"
#include<vector>
#include <unordered_map>
#include"scene/scene.h"
#include"Singleton.h"
class RenderManager:public Singleton<RenderManager>
{

	friend class Singleton<RenderManager>;

public:
	RenderManager(const RenderManager&) = delete;
	RenderManager& operator=(const RenderManager&) = delete;

private:
	RenderManager() = default;

public:
	
	
	std::vector<std::shared_ptr<RenderPass>> renderPipeLine;
	
	std::unordered_map<std::string, unsigned int> textureMap;
	
	
	std::shared_ptr<Scene> scene;
	
	
	void init();
	
	
	void update();
	
	void regisiterRenderPass(std::shared_ptr<RenderPass> pass);
	
	void regisiterScene(std::shared_ptr<Scene> scene);

	void regisiterTexture(std::string s, unsigned int textureId);


	unsigned int getTexture(std::string s);

	int getWidth();

	int getHeight();

	std::shared_ptr<Scene> getScene() {
		return scene;
	}

	GLint ScreenQuadVAO = 0;

	GLint m_SphereVAO = 0;
	GLint m_CubeVAO = 0;

	//基本图形绘制
	GLint getOrCreateScreenQuadVAO();

	GLint getOrCretaeSphereVAO();

	GLint getOrCreateCubeVAO();






};

