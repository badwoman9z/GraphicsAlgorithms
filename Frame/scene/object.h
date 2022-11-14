#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Type
{
	Type_Mesh,
	Type_Light
};
class Object
{
public:
	bool isEnabled = true;
	char name[32];
	glm::mat4 model = glm::mat4(1);
	virtual Type GetType() = 0;
	void SetName(const char* name);



};
