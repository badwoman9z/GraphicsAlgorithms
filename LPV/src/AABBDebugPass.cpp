#include"AABBDebugPass.h"
void AABBDebugPass::drawVPLVAO()
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
void AABBDebugPass::init() {
	scene = RenderManager::get_instance().getScene();
	rsmResolution = scene->RSMResolution;
	m_Min = scene->getOrCreateBounding()->getMin();
	m_Max = scene->getOrCreateBounding()->getMax();
	Dimensions = m_Max - m_Min;
	shader->use();
	shader->setVec3("u_Dimensions", Dimensions);
	shader->setFloat("u_CellSize", 1.0);
	shader->setVec3("u_MinAABB", m_Min);
	
	shader->setInt("LPVGridR", 1);
	shader->setInt("LPVGridG", 2);
	shader->setInt("LPVGridB", 3);
	shader->setInt("rsmPosition", 4);
	shader->setInt("rsmNormal", 5);
	shader->setInt("u_RSMResolution", scene->RSMResolution);

}

void AABBDebugPass::update() {
	//glm::ivec3 begin = m_Max - m_Min;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, RenderManager::get_instance().getWidth(), RenderManager::get_instance().getHeight());
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);
	shader->use();
	glm::mat4 view = scene->view;
	glm::mat4 projection = scene->projection;
	shader->setMat4("view", view);
	shader->setMat4("projection", projection);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_3D, RenderManager::get_instance().getTexture("LPVGridR"));
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_3D, RenderManager::get_instance().getTexture("LPVGridG"));
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_3D, RenderManager::get_instance().getTexture("LPVGridB"));
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, RenderManager::get_instance().getTexture("rsmPosition"));
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, RenderManager::get_instance().getTexture("rsmNormal"));
	drawVPLVAO();
	//for (int i = m_Min.x; i < m_Max.x; i++)
	//	for (int j = m_Min.y; j < m_Max.y; j++)
	//		for (int k = m_Min.z; k < m_Max.z; k++)
	//		{
	//			glm::mat4 model = glm::mat4(1);
	//			model = glm::translate(model, glm::vec3(i, j, k));
	//			model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
	//			shader->setMat4("u_ModelMatrix", model);
	//			drawSphere();
	//		}



	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}