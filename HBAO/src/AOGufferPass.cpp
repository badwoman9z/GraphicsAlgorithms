#include "AOGufferPass.h"

void AOGufferPass::init()
{	
	scene = RenderManager::get_instance().getScene();
	gbufferTexture = std::make_shared<Texture>(GL_RGBA32F, GL_FLOAT, GL_RGBA, scene->SCR_WIDTH, scene->SCR_HEIGHT);
	depthTexture = std::make_shared<Texture>(GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_COMPONENT, scene->SCR_WIDTH, scene->SCR_HEIGHT);
	normal = std::make_shared<Texture>(GL_RGB16F, GL_FLOAT, GL_RGB, scene->SCR_WIDTH, scene->SCR_HEIGHT);
	RenderManager::get_instance().regisiterTexture("gbuffer", gbufferTexture->ID);
	RenderManager::get_instance().regisiterTexture("gNormal", normal->ID);
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gbufferTexture->ID, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normal->ID, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture->ID, 0);
	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
	glDrawBuffers(2, attachments);

	//glGenRenderbuffers(1, &RBO);
	//glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, scene->SCR_WIDTH, scene->SCR_HEIGHT);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void AOGufferPass::update()
{
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, scene->SCR_WIDTH, scene->SCR_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 model(1);
	//model = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.05f)), glm::vec3(0.0f, 0.0f, 0.0f));
	shader->use();
	shader->setMat4("model", model);
	scene->Draw(*shader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);

}
