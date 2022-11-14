#include "PropagationPass.h"

void PropagationPass::drawVPLVAO()
{
	if (m_VPLVAO == -1)
	{
		std::vector<glm::vec3> Vertices;
		for (int i = 0; i < Dimensions.x; i++)
			for (int j = 0; j < Dimensions.y; j++)
				for (int k = 0; k < Dimensions.z; k++)
					Vertices.push_back(glm::vec3(i, j, k));

		std::cout << "ssssssssss" << Dimensions.x << Dimensions.y << Dimensions.z << std::endl;
		m_VPLVAO = createVAO(&Vertices.front(), Vertices.size() * 3 * sizeof(float), { 3 });
	}
	glBindVertexArray(m_VPLVAO);
	glDrawArrays(GL_POINTS, 0, Dimensions.x * Dimensions.y * Dimensions.z);
	glBindVertexArray(0);
}
void PropagationPass::init()
{
	scene = RenderManager::get_instance().getScene();
	glm::vec3 MinAABB = scene->getOrCreateBounding()->getMin();
	glm::vec3 MaxAABB = scene->getOrCreateBounding()->getMax();
	Dimensions = MaxAABB - MinAABB;
	LPVRPipeLine.push_back(std::make_shared<Texture>(RenderManager::get_instance().getTexture("LPVGridR")));
	LPVGPipeLine.push_back(std::make_shared<Texture>(RenderManager::get_instance().getTexture("LPVGridG")));
	LPVBPipeLine.push_back(std::make_shared<Texture>(RenderManager::get_instance().getTexture("LPVGridB")));

	for (int i = 1; i <= m_ProgationNum; i++) {

		LPVRPipeLine.push_back(std::make_shared<Texture>(GL_TEXTURE_3D, GL_RGBA16F, GL_FLOAT, GL_RGBA, Dimensions.x, Dimensions.y, Dimensions.z));
		LPVGPipeLine.push_back(std::make_shared<Texture>(GL_TEXTURE_3D, GL_RGBA16F, GL_FLOAT, GL_RGBA, Dimensions.x, Dimensions.y, Dimensions.z));
		LPVBPipeLine.push_back(std::make_shared<Texture>(GL_TEXTURE_3D, GL_RGBA16F, GL_FLOAT, GL_RGBA, Dimensions.x, Dimensions.y, Dimensions.z));
	}
	LPVAccumulatorR = std::make_shared<Texture>(GL_TEXTURE_3D, GL_RGBA16F, GL_FLOAT, GL_RGBA, Dimensions.x, Dimensions.y, Dimensions.z);
	LPVAccumulatorG = std::make_shared<Texture>(GL_TEXTURE_3D, GL_RGBA16F, GL_FLOAT, GL_RGBA, Dimensions.x, Dimensions.y, Dimensions.z);
	LPVAccumulatorB = std::make_shared<Texture>(GL_TEXTURE_3D, GL_RGBA16F, GL_FLOAT, GL_RGBA, Dimensions.x, Dimensions.y, Dimensions.z);
	RenderManager::get_instance().regisiterTexture("LPVAccumulatorR", LPVAccumulatorR->ID);
	RenderManager::get_instance().regisiterTexture("LPVAccumulatorG", LPVAccumulatorG->ID);
	RenderManager::get_instance().regisiterTexture("LPVAccumulatorB", LPVAccumulatorB->ID);


	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);



	shader->use();
	shader->setVec3("u_Dimensions", Dimensions);
	shader->setInt("Pre_LPVGridR", 0);
	shader->setInt("Pre_LPVGridG", 1);
	shader->setInt("Pre_LPVGridB", 2);

}

void PropagationPass::update()
{
	for (int i = 1; i < m_ProgationNum; i++) {
		ClearTexture(Dimensions.x, Dimensions.y, Dimensions.z, GL_TEXTURE_3D, LPVRPipeLine[i]->ID);
		ClearTexture(Dimensions.x, Dimensions.y, Dimensions.z, GL_TEXTURE_3D, LPVGPipeLine[i]->ID);
		ClearTexture(Dimensions.x, Dimensions.y, Dimensions.z, GL_TEXTURE_3D, LPVBPipeLine[i]->ID);
	}
	ClearTexture(Dimensions.x, Dimensions.y, Dimensions.z, GL_TEXTURE_3D, LPVAccumulatorR->ID);
	ClearTexture(Dimensions.x, Dimensions.y, Dimensions.z, GL_TEXTURE_3D, LPVAccumulatorG->ID);
	ClearTexture(Dimensions.x, Dimensions.y, Dimensions.z, GL_TEXTURE_3D, LPVAccumulatorB->ID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, Dimensions.x, Dimensions.y);
	shader->use();
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,LPVAccumulatorR->ID,0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, LPVAccumulatorG->ID, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,  LPVAccumulatorB->ID, 0);
	bool isFist = true;

	for (int i = 1; i <=m_ProgationNum; i++) {

		if (i != 1) {
			isFist = false;
		}
		shader->setBool("isFist", isFist);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_3D, LPVRPipeLine[i-1]->ID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_3D, LPVGPipeLine[i-1]->ID);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_3D, LPVBPipeLine[i-1]->ID);


		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3,  LPVRPipeLine[i]->ID, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, LPVGPipeLine[i]->ID, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5,LPVBPipeLine[i]->ID,0);
		GLuint attachments[6] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5 };
		glDrawBuffers(6, attachments);

		drawVPLVAO();

	}
	glDisable(GL_BLEND);
	glViewport(0, 0, scene->SCR_WIDTH, scene->SCR_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
