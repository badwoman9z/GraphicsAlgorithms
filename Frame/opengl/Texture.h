#pragma once
#include<GLEW/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include"stb_image.h"

class Texture
{
public:
	unsigned int ID;
	std::string path;
	std::string name;
	unsigned char* bytes = nullptr;
	float* data = nullptr;
	int width, height, numColCh;


	Texture(const char* path, GLenum type = GL_UNSIGNED_BYTE, GLenum wrap = GL_REPEAT, bool mipmap = false);
	Texture(GLenum internalFormat, GLenum type, GLenum format,int w,int h);
	Texture(GLenum textureType, GLenum internalFormat, GLenum type, GLenum format, GLsizei w, GLsizei  h, GLsizei depth, bool mipmap = false);
	Texture(unsigned int id) :ID(id) {};



	

	~Texture();
	void Bind(GLenum slot);

private:

};

