#pragma once
#include<GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#define PI 3.1415926f
 std::string GetFileNameFromPath(const std::string& s);


 GLint  createVAO(const GLvoid* vVertices, GLint vVerticesSize, std::initializer_list<GLint> vAttribsLength, const GLint vIndices[] = nullptr, GLint vIndicesSize = 0, int* voVBO = nullptr);
 GLint  createVAO(int vBufferIndex, std::initializer_list<GLint> vAttribsLength, GLenum vDataType = GL_FLOAT);

 GLint  createVAO4ScreenQuad();
 GLint createVAO4Cube();
 void   drawQuad();
 GLint  genBuffer(GLenum vTarget, GLsizeiptr vSize, const GLvoid* vData, GLenum vUsage, GLint vBindingIndex = -1);
 GLvoid ClearTexture(GLsizei w, GLsizei  h, GLsizei depth, GLuint TextureType, unsigned int TextureID);

 void drawSphere();
 void drawCube();
 GLint createVAO4Sphere();
