#include"RSMBufferPass.h"

void RSMBufferPass::init() {
	scene = RenderManager::get_instance().getScene();
	scene->RSMResolution = RSMResolution;

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	rsm_position = std::make_shared<Texture>(GL_RGBA32F, GL_FLOAT, GL_RGBA, RSMResolution, RSMResolution);
	GLuint attachments[1] = { GL_COLOR_ATTACHMENT0 };
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rsm_position->ID, 0);
	
	
	glDrawBuffers(1, attachments);

	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, RSMResolution, RSMResolution);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

	RenderManager::get_instance().regisiterTexture("rsmPosition", rsm_position->ID);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	
}
void RSMBufferPass::update() {
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glViewport(0, 0, RSMResolution, RSMResolution);

	shader->use();

	glm::mat4 model = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.05f)), glm::vec3(0.0f, 0.0f, 0.0f));
	shader->setMat4("model", model);
	shader->setMat4("u_LightVPMatrix", scene->light.getLightVPMatrix());
	scene->Draw(*shader);

	glViewport(0, 0, scene->SCR_WIDTH, scene->SCR_HEIGHT);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}