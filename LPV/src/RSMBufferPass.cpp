#include"RSMBufferPass.h"

void RSMBufferPass::init() {
	scene = RenderManager::get_instance().getScene();
	scene->RSMResolution = RSMResolution;

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	rsm_flux = std::make_shared<Texture>(GL_RGBA32F, GL_FLOAT, GL_RGBA, RSMResolution, RSMResolution);
	rsm_normal = std::make_shared<Texture>(GL_RGB32F, GL_FLOAT, GL_RGB, RSMResolution, RSMResolution);
	rsm_position = std::make_shared<Texture>(GL_RGBA32F, GL_FLOAT, GL_RGBA, RSMResolution, RSMResolution);
	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rsm_flux->ID, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, rsm_normal->ID, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, rsm_position->ID, 0);
	
	
	glDrawBuffers(3, attachments);

	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, RSMResolution, RSMResolution);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);


	RenderManager::get_instance().regisiterTexture("rsmFlux", rsm_flux->ID);
	RenderManager::get_instance().regisiterTexture("rsmNormal", rsm_normal->ID);
	RenderManager::get_instance().regisiterTexture("rsmPosition", rsm_position->ID);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	
}
void RSMBufferPass::update() {
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glViewport(0, 0, RSMResolution, RSMResolution);
	glm::mat4 model = glm::mat4(1);
	shader->use();
	shader->setMat4("model", model);
	shader->setMat4("u_LightVPMatrix", scene->light.getLightVPMatrix());
	shader->setVec3("u_LightIntensity", scene->light.Intensity);
	shader->setVec3("u_LightDir", scene->light.LightDir);

	scene->Draw(*shader);

	glViewport(0, 0, scene->SCR_WIDTH, scene->SCR_HEIGHT);
	//glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}