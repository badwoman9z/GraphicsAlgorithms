#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Light
{
public:
	glm::vec3 LightPos;
	glm::vec3 Intensity;
	glm::vec3 LightDir;
	glm::vec3 LightUpVector = glm::vec3(0, 1, 0);
	std::vector<glm::mat4> frustumVPMat;
	std::vector<float> frustumFarPlane;
	float size = 120.0;
	Light(glm::vec3 P, glm::vec3 I) :LightPos(P), Intensity(I) {
		//LightDir = glm::vec3(0, 3, 0);
		LightDir = glm::vec3(0, 1, 0.25);

		//LightDir = glm::vec3(0.134, -1.0, 0.146);
		//glm::rotation(LightDir, glm::vec3(78, -10.3, 0.0));
	}
	Light() {}

	glm::mat4 getLightVPMatrix() {
		glm::mat4 LightViewMatrix = glm::lookAt(LightDir, glm::vec3(0, 0, 0), LightUpVector);
		glm::mat4 LightProjectionMatrix = glm::ortho(-size, size, -size, size, -100.0f, 100.0f);
		return LightProjectionMatrix * LightViewMatrix;
	}
	glm::mat4 getLightViewMatrix() {
		return glm::lookAt(LightDir, glm::vec3(0, 0, 0), LightUpVector);;
	}

};
