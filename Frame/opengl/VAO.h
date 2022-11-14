#pragma once
#include<GLEW/glew.h>
#include"VBO.h"


class VAO
{
public:
	unsigned int ID;
	void LinkAttrib(VBO& vbo, unsigned int layout, unsigned int numComponents, GLenum type, GLsizei stride, void* offset) {
		vbo.Bind();
		glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
		glEnableVertexAttribArray(layout);
		vbo.Unbind();
	}
	void Bind() {
		glBindVertexArray(ID);
	}
	void Unbind() {
		glBindVertexArray(0);
	}
	void Delete() {
		glDeleteVertexArrays(1, &ID);
	}
	VAO() {
		glGenVertexArrays(1, &ID);
		
	}


};


