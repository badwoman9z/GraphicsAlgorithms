#include "HBAOPass.h"

void HBAOPass::init()
{
	scene = RenderManager::get_instance().getScene();
	unsigned int ID;
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R16, 4, 4, 0, GL_RED, GL_FLOAT, generateNoise());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
	noiseTexture = std::make_shared<Texture>(ID);
	
	glm::vec2 FocalLen, InvFocalLen, UVToViewA, UVToViewB, LinMAD;
	float fovRad = glm::radians(scene->camera->Zoom);
	FocalLen[0] = 1.0f / tanf(fovRad * 0.5f) * ((float)scene->SCR_HEIGHT / (float)scene->SCR_WIDTH);
	FocalLen[1] = 1.0f / tanf(fovRad * 0.5f);

	
	shader->use();
	shader->setInt("gbuffer", 0);
	shader->setInt("u_NoiseTexture", 1);
	shader->setFloat("u_FOV", 1.0f / tanf(fovRad * 0.5f));
}

void HBAOPass::update()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, RenderManager::get_instance().getTexture("gbuffer"));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, noiseTexture->ID);
	shader->use();
	shader->setMat4("projection", scene->projection);
	//shader->setFloat("radius", 3.0);
	drawQuad();
}

float* HBAOPass::generateNoise()
{
	float* noise = new float[4 * 4 * 4];
	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			glm::vec2 xy = glm::circularRand(1.0f);
			float z = glm::linearRand(0.0f, 1.0f);
			float w = glm::linearRand(0.0f, 1.0f);

			int offset = 4 * (y * 4 + x);
			noise[offset + 0] = xy[0];
			noise[offset + 1] = xy[1];
			noise[offset + 2] = z;
			noise[offset + 3] = w;
		}
	}
	return noise;
}
