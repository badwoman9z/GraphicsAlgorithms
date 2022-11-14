#include "utils.h"
#include"render/RenderManager.h"
#include <numeric>
std::string GetFileNameFromPath(const std::string& s)
{
	auto slash = s.find_last_of('/');
	if (slash == -1)
		slash = s.find_last_of('\\');
	auto dot = s.find_last_of('.');
	return s.substr(slash + 1, dot - slash - 1);
}

GLint createVAO(const GLvoid* vVertices, GLint vVerticesSize, std::initializer_list<GLint> vAttribsLength, const GLint vIndices[], GLint vIndicesSize, int* voVBO)
{
	GLint VAO = 0, VBO = 0, EBO = 0;
	glGenVertexArrays(1, &(GLuint&)VAO);
	glBindVertexArray(VAO);
	VBO = genBuffer(GL_ARRAY_BUFFER, vVerticesSize, vVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  //In function "genBuffer", bindbuffer(0) is called, so here we must rebind so that VAO is effective
	if (vIndices)
	{
		EBO = genBuffer(GL_ELEMENT_ARRAY_BUFFER, vIndicesSize, vIndices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	}
	GLint AttribOffset = 0;
	GLint i = 0;
	GLint Stride = std::accumulate(vAttribsLength.begin(), vAttribsLength.end(), 0);
	for (auto vAttribLength : vAttribsLength)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, vAttribLength, GL_FLOAT, GL_FALSE, Stride * sizeof(GL_FLOAT), (GLvoid*)(AttribOffset * sizeof(GL_FLOAT)));
		AttribOffset += vAttribLength;
		++i;
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	if (voVBO)
		*voVBO = VBO;
	return VAO;
}

GLint createVAO(int vBufferIndex, std::initializer_list<GLint> vAttribsLength, GLenum vDataType)
{
	GLint VAO = 0;
	glGenVertexArrays(1, &(GLuint&)VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, vBufferIndex);
	GLint AttribOffset = 0;
	GLint i = 0;
	GLint Stride = std::accumulate(vAttribsLength.begin(), vAttribsLength.end(), 0);
	for (auto vAttribLength : vAttribsLength)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, vAttribLength, vDataType, GL_FALSE, Stride * sizeof(vDataType), (GLvoid*)(AttribOffset * sizeof(vDataType)));
		AttribOffset += vAttribLength;
		++i;
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return VAO;
}

GLint createVAO4ScreenQuad()
{
	GLfloat WindowQuadVertices[] = {
	1.0f, 1.0f,  1.0f,1.0f,
	1.0f,-1.0f,  1.0f,0.0f,
	-1.0f, 1.0f,  0.0f,1.0f,
	-1.0f, 1.0f,  0.0f,1.0f,
	1.0f,-1.0f,  1.0f,0.0f,
	-1.0f,-1.0f,  0.0f,0.0f
	};
	return createVAO(WindowQuadVertices, sizeof(WindowQuadVertices), { 2,2 });
}

void drawQuad()
{
	
	glBindVertexArray(RenderManager::get_instance().getOrCreateScreenQuadVAO());
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
GLint genBuffer(GLenum vTarget, GLsizeiptr vSize, const GLvoid* vData, GLenum vUsage, GLint vBindingIndex)
{
	GLint BufferID;
	glGenBuffers(1, &(GLuint&)BufferID);
	glBindBuffer(vTarget, BufferID);
	glBufferData(vTarget, vSize, vData, vUsage);
	glBindBuffer(vTarget, 0);
	if (vBindingIndex != -1)
		glBindBufferBase(vTarget, vBindingIndex, BufferID);
	return BufferID;
}
GLvoid ClearTexture(GLsizei w, GLsizei  h, GLsizei depth, GLuint TextureType,unsigned int TextureID)
{
	std::vector<GLfloat> emptyData(w* h * depth * sizeof(float), 0.0);
	glBindTexture(TextureType, TextureID);
	//glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA16F, MAX_GRID_SIZE, MAX_GRID_SIZE, MAX_GRID_SIZE, 0, GL_RGBA, GL_FLOAT, &emptyData[0]);
	//or
	glTexSubImage3D(TextureType, 0, 0, 0, 0, w, h, depth, GL_RGBA, GL_FLOAT, &emptyData[0]);
	glBindTexture(TextureType, 0);
}




//************************************************************************************
//Function:
void drawSphere()
{
	glBindVertexArray(RenderManager::get_instance().getOrCretaeSphereVAO());
	glDrawElements(GL_TRIANGLE_STRIP, 8320, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

//************************************************************************************
//Function:
GLint createVAO4Sphere()
{
	GLuint SphereVAO = 0;
	glGenVertexArrays(1, &SphereVAO);
	GLuint VBO, EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	std::vector<glm::vec3> Positions;
	std::vector<glm::vec2> UV;
	std::vector<glm::vec3> Normals;
	std::vector<unsigned int> Indices;
	const unsigned int SEGMENTX = 64;
	const unsigned int SEGMENTY = 64;
	for (unsigned int y = 0; y <= SEGMENTY; ++y)
	{
		for (unsigned int x = 0; x <= SEGMENTX; ++x)
		{
			float XSegment = (float)x / (float)SEGMENTX;
			float YSegment = (float)y / (float)SEGMENTY;
			float PosX = std::cos(XSegment * 2.0f * PI) * std::sin(YSegment * PI);
			float PosY = std::cos(YSegment * PI);
			float PosZ = std::sin(XSegment * 2.0f * PI) * std::sin(YSegment * PI);
			Positions.push_back(glm::vec3(PosX, PosY, PosZ));
			UV.push_back(glm::vec2(XSegment, YSegment));
			Normals.push_back(glm::vec3(PosX, PosY, PosZ));
		}
	}
	bool OddRow = false;
	for (int y = 0; y < SEGMENTY; ++y)
	{
		if (!OddRow) // even rows: y == 0, y == 2; and so on
		{
			for (int x = 0; x <= SEGMENTX; ++x)
			{
				Indices.push_back(y * (SEGMENTX + 1) + x);
				Indices.push_back((y + 1) * (SEGMENTX + 1) + x);
			}
		}
		else
		{
			for (int x = SEGMENTX; x >= 0; --x)
			{
				Indices.push_back((y + 1) * (SEGMENTX + 1) + x);
				Indices.push_back(y * (SEGMENTX + 1) + x);
			}
		}
		OddRow = !OddRow;
	}
	std::vector<float> Data;
	for (size_t i = 0; i < Positions.size(); ++i)
	{
		Data.push_back(Positions[i].x);
		Data.push_back(Positions[i].y);
		Data.push_back(Positions[i].z);
		if (UV.size() > 0)
		{
			Data.push_back(UV[i].x);
			Data.push_back(UV[i].y);
		}
		if (Normals.size() > 0)
		{
			Data.push_back(Normals[i].x);
			Data.push_back(Normals[i].y);
			Data.push_back(Normals[i].z);
		}
	}
	glBindVertexArray(SphereVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Data.size() * sizeof(float), &Data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);
	int Stride = static_cast<int>((3 + 2 + 3) * sizeof(float));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Stride, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, Stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, Stride, (void*)(5 * sizeof(float)));

	return SphereVAO;
}
GLint createVAO4Cube()
{
	GLfloat CubeVertices[] = {
		// back face
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
		// front face
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
		1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		// left face
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		// right face
		1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
		1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
		// bottom face
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
		1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		// top face
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
		1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left      
	};
	return createVAO(CubeVertices, sizeof(CubeVertices), { 3,3,2 });
}
//Function:
void drawCube()
{
	glBindVertexArray(RenderManager::get_instance().getOrCreateCubeVAO());
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}