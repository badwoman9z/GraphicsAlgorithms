#include"ConeTracingPass.h"
void ConeTracingPass::init()
{
	scene = RenderManager::get_instance().getScene();



	shader->use();
	shader->setVec3("u_LightIntensity", scene->light.Intensity);
	shader->setFloat("VoxelGridWorldSize", 150);
	shader->setFloat("u_VoxelResolution", float(scene->VoxelResolution));


	shader->setInt("rsmPosition", 2);
	shader->setInt("textureColor", 3);



}

void ConeTracingPass::update()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, scene->SCR_WIDTH, scene->SCR_HEIGHT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	shader->use();

	shader->setMat4("u_LightVPMatrix", scene->light.getLightVPMatrix());
	shader->setVec3("u_LightDir", scene->light.LightDir);
	shader->setVec3("cameraPos", scene->camera->Position);
	glm::mat4 model = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.05f)), glm::vec3(0.0f, 0.0f, 0.0f));
	shader->setMat4("model", model);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, RenderManager::get_instance().getTexture("rsmPosition"));

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_3D, RenderManager::get_instance().getTexture("VoxelTexture"));
	scene->Draw(*shader);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);






}





