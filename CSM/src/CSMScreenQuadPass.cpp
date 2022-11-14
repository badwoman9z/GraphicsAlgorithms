#include "CSMScreenQuadPass.h"
#include "utils.h"
void CSMScreenQuadPass::init() {
	//glm::vec3 min = RenderManager::get_instance().getScene()->bounding->getMin();
	shader->use();
	shader->setInt("rsmFlux", 2);

}


void CSMScreenQuadPass::update() {
	glm::vec2 translations[12];
	int index = 0;
	float offset = -25.0f;
	translations[0] = glm::vec2(0);
	for (index = 1; index < 12; index++) {
		translations[index] = glm::vec2(offset);
		offset += 5;
		std::cout << offset << std::endl;
	}
	//translations[1] = glm::vec2(5);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, RenderManager::get_instance().getWidth(), RenderManager::get_instance().getHeight());
	
	shader->use();

	for (unsigned int i = 0; i < 12; i++)
	{
		shader->setVec2("offsets[" + std::to_string(i) + "]", translations[i]);
	}
	glm::mat4 model = glm::mat4(1.0);
	//glm::mat4 model = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.05f)), glm::vec3(0.0f, 0.0f, 0.0f));
	shader->setMat4("model", model);
	for (int i = 0; i < 4; i++) {
		shader->setFloat("farPlanes[" + std::to_string(i) + "]", scene->light.frustumFarPlane[i]);
		
		shader->setMat4("u_lightProjection[" + std::to_string(i) + "]", scene->light.frustumVPMat[i]);	
		//std::cout << glm::to_string(scene->light.frustumVPMat[i]) << std::endl;
	}
	shader->setVec3("lightDir", scene->light.LightDir);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D_ARRAY, RenderManager::get_instance().getTexture("depthTexture"));
	//scene->Draw(*shader);
	scene->DrawInstance(*shader);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
