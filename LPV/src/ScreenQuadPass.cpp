#include "ScreenQuadPass.h"
#include "utils.h"
void ScreenQuadPass::init() {
	glm::vec3 min = RenderManager::get_instance().getScene()->bounding->getMin();
	shader->use();
	shader->setFloat("u_CellSize", 1.0f);
	shader->setVec3("u_MinAABB", min);
	shader->setInt("directColor",0 );
	shader->setInt("normal", 1);
	shader->setInt("position", 2);
/*	shader->setInt("LPVAccumulatorR",3);
	shader->setInt("LPVAccumulatorG",4);
	shader->setInt("LPVAccumulatorB",5)*/;
	shader->setInt("gbufferColor", 6);
	shader->setInt("indirectLight", 7);
}


void ScreenQuadPass::update() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, RenderManager::get_instance().getWidth(), RenderManager::get_instance().getHeight());
	shader->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, RenderManager::get_instance().getTexture("directColor"));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, RenderManager::get_instance().getTexture("gbufferNormal"));
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, RenderManager::get_instance().getTexture("gbufferPosition"));

	//glActiveTexture(GL_TEXTURE3);
	//glBindTexture(GL_TEXTURE_3D, RenderManager::get_instance().getTexture("LPVAccumulatorR"));
	//glActiveTexture(GL_TEXTURE4);
	//glBindTexture(GL_TEXTURE_3D, RenderManager::get_instance().getTexture("LPVAccumulatorG"));
	//glActiveTexture(GL_TEXTURE5);
	//glBindTexture(GL_TEXTURE_3D, RenderManager::get_instance().getTexture("LPVAccumulatorB"));

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, RenderManager::get_instance().getTexture("gbufferColor"));
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, RenderManager::get_instance().getTexture("indirectLight"));
	drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
