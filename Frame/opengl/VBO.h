#pragma once
#include<GLEW/glew.h>
#include <vector>
#include "vertex.h"
#include <GLFW/glfw3.h>

class VBO
{
public:
	unsigned int ID;
	VBO(std::vector<Vertex>& vertices) {
		glGenBuffers(1, &ID);
		glBindBuffer(GL_ARRAY_BUFFER, ID);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	}

	void Bind() {
		glBindBuffer(GL_ARRAY_BUFFER, ID);
	}
	void Unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void Delete() {
		glDeleteBuffers(1, &ID);
	}



};

