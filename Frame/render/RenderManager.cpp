#include"RenderManager.h"
#include"utils.h"

void RenderManager::regisiterRenderPass(std::shared_ptr<RenderPass> pass) {
	renderPipeLine.push_back(pass);
}

void RenderManager::regisiterScene(std::shared_ptr<Scene> s) {
	scene = s;
}

void RenderManager::regisiterTexture(std::string s, unsigned int textureId) {
	textureMap[s] = textureId;
}

unsigned int RenderManager::getTexture(std::string s) {
	return textureMap[s];
}

int RenderManager::getWidth() {
	return scene->SCR_WIDTH;
}

int RenderManager::getHeight()
{
	return scene->SCR_HEIGHT;
}

void RenderManager::init() {
	scene->Init();

	for (auto pass : renderPipeLine) {
		pass->init();
	}
}

void RenderManager::update() {
	scene->Update();
	for (auto pass : renderPipeLine) {
		//std::cout << pass->passName << std::endl;
		pass->update();
	}
}


  

GLint RenderManager::getOrCreateScreenQuadVAO() {
	if (ScreenQuadVAO == 0)      //Fixed Me: some method to simply these code
		ScreenQuadVAO = createVAO4ScreenQuad();
	return ScreenQuadVAO;
}

GLint RenderManager::getOrCretaeSphereVAO()
{
	if (m_SphereVAO == 0)
		m_SphereVAO = createVAO4Sphere();

	
	return m_SphereVAO;
}
GLint RenderManager::getOrCreateCubeVAO()
{
	if (m_CubeVAO == 0)
		m_CubeVAO = createVAO4Cube();
	return m_CubeVAO;
}
