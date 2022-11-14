#include "VoxelvisualizePass.h"
void VoxelvisualizePass::RenderCube() {
    // Initialize (if necessary)
    if (cubeVAO == 0)
    {
        GLfloat vertices[] = {
                   -1, -1, 1,0.5, 1, 1
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // Fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // Link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // Render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);


}
void VoxelvisualizePass::init()
{
	VoxelResolution = RenderManager::get_instance().getScene()->VoxelResolution;
	glm::vec3 min = RenderManager::get_instance().getScene()->bounding->getMin();
	glm::vec3 max = RenderManager::get_instance().getScene()->bounding->getMax();
	glm::vec3 Dimensions = max - min;
	shader->use();
	glm::vec3 scale = glm::vec3((2.0 - 0.1) / fabs(Dimensions.x), (2.0 - 0.1) / fabs(Dimensions.y), (2.0 - 0.1) / fabs(Dimensions.z));
	shader->setInt("textureFront", 0);
	shader->setInt("textureBack", 1);
    shader->setInt("textureColor", 2);
}

void VoxelvisualizePass::update()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, RenderManager::get_instance().getScene()->SCR_WIDTH, RenderManager::get_instance().getScene()->SCR_HEIGHT);
	shader->use();
    shader->setVec3("cameraPosition", RenderManager::get_instance().getScene()->camera->Position);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, RenderManager::get_instance().getTexture("textureFront"));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, RenderManager::get_instance().getTexture("textureBack"));
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_3D, RenderManager::get_instance().getTexture("VoxelTexture"));
	drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
