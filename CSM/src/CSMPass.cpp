#include"CSMPass.h"

void CSMPass::init()
{
	frustumProjectionMat.resize(4);
	scene->light.frustumVPMat.resize(4);


	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	//updateFrustum();
	depthTexture = std::make_shared<Texture>(GL_TEXTURE_2D_ARRAY, GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_COMPONENT, resulotion, resulotion, 4);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture->ID, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	RenderManager::get_instance().regisiterTexture("depthTexture", depthTexture->ID);


}

void CSMPass::update()
{		

	glm::vec2 translations[12];
	int index = 0;
	translations[0] = glm::vec2(0);
	
	float offset = -25.0f;
	
	for (index = 1; index < 12; index++) {
		translations[index] = glm::vec2(offset);
		offset += 5;
	}

		updateOuterSphereFrustum();
		//updateCompactFrustum();
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
		glViewport(0, 0, resulotion, resulotion);
		shader->use();

		for (unsigned int i = 0; i < 12; i++)
		{
			shader->setVec2("offsets[" + std::to_string(i) + "]", translations[i]);
		}



		glm::mat4 model = glm::mat4(1.0);
		//glm::mat4 model = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.05f)), glm::vec3(0.0f, 0.0f, 0.0f));
		shader->setMat4("model", model);
		for (int i = 0; i < 4; i++) {
			shader->setMat4("u_LightVPMatrix[" + std::to_string(i) + "]", glm::mat4(frustumProjectionMat[i]));
		}
		//scene->Draw(*shader);
		scene->DrawInstance(*shader);
		glViewport(0, 0, scene->SCR_WIDTH, scene->SCR_HEIGHT);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		//std::cout << "update" << std::endl;



}

void CSMPass::updateCompactFrustum()
{
	std::vector<glm::vec3> ndcPos = { glm::vec3(-1.0f, -1.0f, -1.0f),glm::vec3(1.0f, -1.0f, -1.0f) ,glm::vec3(-1.0f, 1.0f, -1.0f) ,glm::vec3(1.0f, 1.0f, -1.0f) ,glm::vec3(-1.0f, -1.0f, 1.0f) ,glm::vec3(1.0f, -1.0f, 1.0f) ,
	glm::vec3(-1.0f, 1.0f, 1.0f),glm::vec3(1.0f, 1.0f, 1.0f) };

	glm::mat4 view = scene->view;

	for (int i = 0; i < 4; i++) {
	
		glm::mat4 projection = glm::perspective(glm::radians(scene->camera->Zoom), (float)scene->SCR_WIDTH / (float)scene->SCR_HEIGHT, frustum[i].Near, frustum[i].Far);
		
		glm::mat4 mat = glm::inverse(projection * view);

		std::vector<glm::vec4> corners;

		for (int j = 0; j < 8; j++) {
			
			glm::vec4 clipPos = mat * glm::vec4(ndcPos[j],1.0);

			corners.push_back(clipPos / clipPos.w);


		}


		glm::vec3 aabb_max(-1000.0f, -1000.0f, -1000.0f);
		glm::vec3 aabb_min(1000.0f, 1000.0f, 1000.0f);

		for (int j = 0; j < 8; j++) {

			glm::vec4 viewPos = scene->light.getLightViewMatrix() * corners[j];

			if (viewPos.z > aabb_max.z) { aabb_max.z = viewPos.z; }
			if (viewPos.z < aabb_min.z) { aabb_min.z = viewPos.z; }
			if (viewPos.x > aabb_max.x) { aabb_max.x = viewPos.x; }
			if (viewPos.x < aabb_min.x) { aabb_min.x = viewPos.x; }
			if (viewPos.y > aabb_max.y) { aabb_max.y = viewPos.y; }
			if (viewPos.y < aabb_min.y) { aabb_min.y = viewPos.y; }

		}

		aabb_max.z += 10;
		std::cout  << std::endl;
		std::cout << glm::to_string(aabb_min) << std::endl;
		std::cout << "--------------------" << std::endl;
		std::cout << glm::to_string(aabb_max) << std::endl;
		std::cout << std::endl;
		frustumProjectionMat[i] = glm::ortho(aabb_min.x, aabb_max.x, aabb_min.y, aabb_max.y, -aabb_max.z, -aabb_min.z) * scene->light.getLightViewMatrix();

		scene->light.frustumVPMat[i] = frustumProjectionMat[i];
	
		corners.clear();
	}
	std::cout << "xxxxxxxxxxxxxxxxxxxxxx" << std::endl;
}

void CSMPass::updateOuterSphereFrustum()
{
	std::vector<glm::vec3> ndcPos = { glm::vec3(-1.0f, -1.0f, -1.0f),glm::vec3(1.0f, -1.0f, -1.0f) ,glm::vec3(-1.0f, 1.0f, -1.0f) ,glm::vec3(1.0f, 1.0f, -1.0f) ,glm::vec3(-1.0f, -1.0f, 1.0f) ,glm::vec3(1.0f, -1.0f, 1.0f) ,
	glm::vec3(-1.0f, 1.0f, 1.0f),glm::vec3(1.0f, 1.0f, 1.0f) };

	glm::mat4 view = scene->view;

	for (int i = 0; i < 4; i++) {
		glm::mat4 projection = glm::perspective(glm::radians(scene->camera->Zoom), (float)scene->SCR_WIDTH / (float)scene->SCR_HEIGHT, frustum[i].Near, frustum[i].Far);

		glm::mat4 mat = glm::inverse(projection * view);

		std::vector<glm::vec4> corners;

		for (int j = 0; j < 8; j++) {

			glm::vec4 clipPos = mat * glm::vec4(ndcPos[j], 1.0);

			corners.push_back(clipPos / clipPos.w);

			//std::cout << glm::to_string(clipPos / clipPos.w) << std::endl;

		}
		float near_len = glm::length2(corners[3] - corners[0]);

		float far_len = glm::length2(corners[7] - corners[4]);

		float len = frustum[i].Far - frustum[i].Near;

		float x = (len / 2.0f) + ((far_len-near_len) / (8 * len));

		float radius = std::sqrt(x * x + near_len*0.25f);

		glm::vec4 lingtViewCenter =  scene->light.getLightViewMatrix()*glm::inverse(view)*glm::vec4(0.0, 0.0, -(frustum[i].Near + x), 1.0);

		
		glm::vec3 frustumCenter = glm::vec3(glm::inverse(view) * glm::vec4(0.0, 0.0, -(frustum[i].Near + x), 1.0));

		
		//glm::mat4 lightView = glm::lookAt(frustumCenter + (scene->light.LightDir * radius), frustumCenter, glm::vec3(0.0, 1.0, 0.0));
		
		
		glm::vec3 aabb_min(lingtViewCenter.x - radius, lingtViewCenter.y - radius,lingtViewCenter.z - radius);
		glm::vec3 aabb_max(lingtViewCenter.x + radius, lingtViewCenter.y + radius, lingtViewCenter.z + radius);



		float fWorldUnitPerTexel = (radius * 2) / resulotion;

		aabb_min /= fWorldUnitPerTexel;
		aabb_min  = glm::floor(aabb_min);
		aabb_min *= fWorldUnitPerTexel;

		aabb_max /= fWorldUnitPerTexel;
		aabb_max = glm::floor(aabb_max);
		aabb_max *= fWorldUnitPerTexel;

		frustumProjectionMat[i] = glm::ortho(aabb_min.x, aabb_max.x, aabb_min.y, aabb_max.y, aabb_min.z-10.f, aabb_max.z+50.0f)*scene->light.getLightViewMatrix();

		scene->light.frustumVPMat[i] = frustumProjectionMat[i];
		corners.clear();
	}
	
}

