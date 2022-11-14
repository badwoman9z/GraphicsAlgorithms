#include"LightInjectPass.h"



void LightInjectPass::drawVPLVAO()
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
void  LightInjectPass::init() {
	scene = RenderManager::get_instance().getScene();
	glm::vec3 min = scene->bounding->getMin();
	glm::vec3 max = scene->bounding->getMax();
	rsmResolution = scene->RSMResolution;
	Dimensions = max - min;
	std::cout << max.r<<"......"<< max.g <<".........."<<max.b<< std::endl;

	std::cout << min.r << "......" << min.g << ".........." << min.b << std::endl;

	//Ê¹ÓÃÖ¡»º³å

	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	LPVGridR = std::make_shared<Texture>(GL_TEXTURE_3D, GL_RGBA16F, GL_FLOAT, GL_RGBA,Dimensions.x,Dimensions.y,Dimensions.z );
	LPVGridG = std::make_shared<Texture>(GL_TEXTURE_3D, GL_RGBA16F, GL_FLOAT, GL_RGBA, Dimensions.x, Dimensions.y, Dimensions.z);
	LPVGridB = std::make_shared<Texture>(GL_TEXTURE_3D, GL_RGBA16F, GL_FLOAT, GL_RGBA, Dimensions.x, Dimensions.y, Dimensions.z);




	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	std::cout << LPVGridR->ID << std::endl;
	std::cout << LPVGridG->ID << std::endl;
	std::cout << LPVGridB->ID << std::endl;
	RenderManager::get_instance().regisiterTexture("LPVGridR", LPVGridR->ID);
	RenderManager::get_instance().regisiterTexture("LPVGridG", LPVGridG->ID);
	RenderManager::get_instance().regisiterTexture("LPVGridB", LPVGridB->ID);
	std::cout << rsmResolution << std::endl;
	shader->use();
	shader->setInt("rsmFlux", 0);
	shader->setInt("rsmNormal", 1);
	shader->setInt("rsmPosition", 2);


	shader->setFloat("u_CellSize", 1.0);
	shader->setVec3("u_MinAABB", min);
	shader->setInt("u_RSMResolution", rsmResolution);
	shader->setVec3("u_Dimensions", Dimensions);

}
void LightInjectPass::update() {
	ClearTexture(Dimensions.x, Dimensions.y, Dimensions.z, GL_TEXTURE_3D, LPVGridR->ID);
	ClearTexture(Dimensions.x, Dimensions.y, Dimensions.z, GL_TEXTURE_3D, LPVGridG->ID);
	ClearTexture(Dimensions.x, Dimensions.y, Dimensions.z, GL_TEXTURE_3D, LPVGridB->ID);
	
	
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	glViewport(0, 0, Dimensions.x, Dimensions.y);
	shader->use();
	glEnable(GL_BLEND);

	glBlendFunc(GL_ONE, GL_ONE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, LPVGridR->ID, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, LPVGridG->ID, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, LPVGridB->ID, 0);

	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, RenderManager::get_instance().getTexture("rsmFlux"));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, RenderManager::get_instance().getTexture("rsmNormal"));
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, RenderManager::get_instance().getTexture("rsmPosition"));
	drawVPLVAO();

	glDisable(GL_BLEND);
	glViewport(0, 0, scene->SCR_WIDTH, scene->SCR_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	
}