#include "ScreenQuadPass.h"
#include "utils.h"
void ScreenQuadPass::init() {
	//glm::vec3 min = RenderManager::get_instance().getScene()->bounding->getMin();
	shader->use();
	shader->setInt("rsmFlux", 0);

}


void ScreenQuadPass::update() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, RenderManager::get_instance().getWidth(), RenderManager::get_instance().getHeight());
	shader->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, RenderManager::get_instance().getTexture("gbufferColor"));
	drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
