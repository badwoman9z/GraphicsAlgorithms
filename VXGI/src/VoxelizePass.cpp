#include"VoxelizePass.h"
#include <utils.h>

void VoxelizePass::init()
{
	scene = RenderManager::get_instance().getScene();
	scene->VoxelResolution = VoxelResolution;
	
	
	voxelTexture = std::make_shared<Texture>(GL_TEXTURE_3D, GL_RGBA8, GL_UNSIGNED_BYTE, GL_RGBA, VoxelResolution, VoxelResolution, VoxelResolution,true);
	glBindImageTexture(0, voxelTexture->ID, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA8);
	RenderManager::get_instance().regisiterTexture("VoxelTexture", voxelTexture->ID);
	

	
	projectMat = glm::ortho(-VoxelWorldSize * 0.5f, VoxelWorldSize * 0.5f, -VoxelWorldSize * 0.5f, VoxelWorldSize * 0.5f, VoxelWorldSize * 0.5f, VoxelWorldSize * 1.5f);
	 projX = projectMat * glm::lookAt(glm::vec3(VoxelWorldSize, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	 projY = projectMat * glm::lookAt(glm::vec3(0, VoxelWorldSize, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));
	 projZ = projectMat * glm::lookAt(glm::vec3(0, 0, VoxelWorldSize), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	std::cout << projX[0][0] << projX[0][1] << projX[0][2] << projX[0][3] <<std::endl;
	std::cout << projX[1][0] << projX[1][1] << projX[1][2] << projX[1][3] << std::endl;
	shader->use();
	shader->setMat4("Projx", projX);
	shader->setMat4("Projy", projY);
	shader->setMat4("Projz", projZ);
	shader->setInt("u_VoxelResolution", int(VoxelResolution));
	shader->setInt("rsmPosition", 2);
}

void VoxelizePass::update()
{

    //ClearTexture(VoxelResolution, VoxelResolution, VoxelResolution, GL_TEXTURE_3D, voxelTexture->ID);
	//ClearTexture(VoxelResolution, VoxelResolution, VoxelResolution, GL_TEXTURE_3D, voxelNormalTexture->ID);
	if (isinit == 0) {
		glViewport(0, 0, VoxelResolution, VoxelResolution);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glDisable(GL_BLEND);
		shader->use();
		shader->setMat4("ProjX", projX);
		shader->setMat4("Projy", projY);
		shader->setMat4("Projz", projZ);
		shader->setInt("u_VoxelResolution", int(VoxelResolution));
		
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, RenderManager::get_instance().getTexture("rsmPosition"));
		
		glm::mat4 model = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.05f)), glm::vec3(0.0f, 0.0f, 0.0f));
		shader->setMat4("model", model);
		shader->setMat4("u_LightVPMatrix", scene->light.getLightVPMatrix());
		shader->setVec3("u_LightDir", scene->light.LightDir);
		glBindImageTexture(0, voxelTexture->ID, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA8);
		scene->Draw(*shader);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_3D, voxelTexture->ID);
		glGenerateMipmap(GL_TEXTURE_3D);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		isinit = 1;


	}

}
