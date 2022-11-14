#include "GeometryInjectPass.h"

void GeometryInjectPass::drawVPLVAO()
{
	if (m_VPLVAO == -1)
	{
		float* VPLVertices = new float[rsmResolution * rsmResolution];
		m_VPLVAO = createVAO(VPLVertices, rsmResolution * rsmResolution, { 2 });
	}
	glBindVertexArray(m_VPLVAO);
	glDrawArrays(GL_POINTS, 0, rsmResolution * rsmResolution);
	glBindVertexArray(0);
}

void GeometryInjectPass::init()
{
	scene = RenderManager::get_instance().getScene();
	rsmResolution = scene->RSMResolution;
	glm::vec3 min = scene->bounding->getMin();
	glm::vec3 max = scene->bounding->getMax();
	Dimensions = max - min;
	GeometryInjection = std::make_shared<Texture>(GL_TEXTURE_3D, GL_RGBA16F, GL_FLOAT, GL_RGBA, Dimensions.x, Dimensions.y, Dimensions.z);
	glBindImageTexture(0, GeometryInjection->ID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA16F);

	RenderManager::get_instance().regisiterTexture("GeometryInjection", GeometryInjection->ID);

	shader->use();
	shader->setInt("rsmNormal", 0);
	shader->setInt("rsmPosition", 1);
	shader->setFloat("u_CellSize", 1);
	shader->setVec3("u_MinAABB", min);
	shader->setInt("u_RSMResolution", rsmResolution);
	shader->setVec3("u_LightDir", RenderManager::get_instance().scene->light.LightDir);
	shader->setMat4("u_LightVPMatrix", RenderManager::get_instance().scene->light.getLightVPMatrix());




}

void GeometryInjectPass::update()
{
	ClearTexture(Dimensions.x, Dimensions.y, Dimensions.z, GL_TEXTURE_3D, GeometryInjection->ID);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Dimensions.x, Dimensions.y);
	shader->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, RenderManager::get_instance().getTexture("rsmNormal"));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, RenderManager::get_instance().getTexture("rsmPosition"));
	drawVPLVAO();
	glViewport(0, 0, scene->SCR_WIDTH, scene->SCR_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
