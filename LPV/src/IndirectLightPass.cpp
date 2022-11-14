#include"IndirectLightPass.h"
#include "utils.h"

void IndirectLightPass::init()
{
	scene = RenderManager::get_instance().getScene();
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	indirectLight = std::make_shared<Texture>(GL_RGBA32F, GL_FLOAT, GL_RGBA, scene->SCR_WIDTH, scene->SCR_HEIGHT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,indirectLight->ID, 0);
	GLuint attachments[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachments);
	RenderManager::get_instance().regisiterTexture("indirectLight", indirectLight->ID);

	glm::vec3 min = RenderManager::get_instance().getScene()->bounding->getMin();
	shader->use();
	shader->setFloat("u_CellSize", 1.0f);
	shader->setVec3("u_MinAABB", min);
	shader->setInt("normal", 1);
	shader->setInt("position", 2);
	shader->setInt("LPVAccumulatorR", 3);
	shader->setInt("LPVAccumulatorG", 4);
	shader->setInt("LPVAccumulatorB", 5);
	shader->setInt("gbufferColor", 6);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void IndirectLightPass::update()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, RenderManager::get_instance().getWidth(), RenderManager::get_instance().getHeight());
	shader->use();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, indirectLight->ID, 0);
	GLuint attachments[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachments);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, RenderManager::get_instance().getTexture("gbufferNormal"));
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, RenderManager::get_instance().getTexture("gbufferPosition"));
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_3D, RenderManager::get_instance().getTexture("LPVAccumulatorR"));
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_3D, RenderManager::get_instance().getTexture("LPVAccumulatorG"));
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_3D, RenderManager::get_instance().getTexture("LPVAccumulatorB"));
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, RenderManager::get_instance().getTexture("gbufferColor"));
	drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
