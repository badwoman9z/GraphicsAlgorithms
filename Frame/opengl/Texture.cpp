#include "Texture.h"
#include<vector>
Texture::~Texture()
{
	if (bytes)
		stbi_image_free(bytes);

	if (data)
		stbi_image_free(data);

	glDeleteTextures(1, &ID);
}
Texture::Texture(const char* path, GLenum type, GLenum wrap, bool mipmap) :path(path)
{

	stbi_set_flip_vertically_on_load(true);
	if (type == GL_FLOAT)
		data = stbi_loadf(path, &width, &height, &numColCh, 0);
	else
		bytes = stbi_load(path, &width, &height, &numColCh, 0);
	std::cout << "Load texture " << name << ", channl count is " << numColCh << std::endl;
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	GLenum format = numColCh == 4 ? GL_RGBA : GL_RGB;
	if (data)
		glTexImage2D(GL_TEXTURE_2D, 0, numColCh == 4 ? GL_RGBA16F : GL_RGB16F, width, height, 0, format, type, data);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, numColCh == 4 ? GL_RGBA8 : GL_RGB8, width, height, 0, format, type, bytes);

	if (mipmap)
		glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}
Texture::Texture(GLenum internalFormat, GLenum type, GLenum format, int w, int h) :width(w), height(h)
{

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);


}

Texture::Texture(GLenum textureType, GLenum internalFormat, GLenum type, GLenum format, GLsizei	 w, GLsizei	 h, GLsizei depth, bool mipmap)
{

	const float b[4] = { 0,0,0,0 };
	glGenTextures(1, &ID);
	std::cout << ID << std::endl;
	glBindTexture(textureType, ID);
	glTexImage3D(textureType, 0, internalFormat, w, h, depth, 0, format, type, NULL);
	glTexParameterfv(textureType, GL_TEXTURE_BORDER_COLOR, b);
	//glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	//glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	if (mipmap) {
		glGenerateMipmap(textureType);
	}
	glBindTexture(textureType, 0);
}

void Texture::Bind(GLenum slot)
{
	glActiveTexture(slot);
	glBindTexture(GL_TEXTURE_2D, ID);
}