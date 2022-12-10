#include"SampleTerrainPass.h"

void SampleTerrainPass::init()
{
	scene = RenderManager::get_instance().getScene();
}

void SampleTerrainPass::update()
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader->use();
	shader->setMat4("projection", scene->projection);
	shader->setMat4("view", scene->view);
	glm::mat4 model = glm::mat4(1.0f);
	shader->setMat4("model", model);
	//terrain->Draw(*shader);
	terrain->DrawElements(*shader);
}
