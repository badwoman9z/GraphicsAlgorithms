#include"Terrain.h"
#include "stb_image.h"
Terrain::Terrain(int type, const char* path)
{
    if (type == 1) {
        LoadTerrianTexture(path);
    }
    else {
        LoadTerrian(path);
    }
}

void Terrain::Draw(Shader shader)
{   
    shader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, heightTexture);
    shader.setInt("heightMap", 0);
    glBindVertexArray(VAO);
    glDrawArrays(GL_PATCHES, 0, 4 * rez * rez);
    glBindVertexArray(0);
}

void Terrain::DrawElements(Shader shader)
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Terrain::LoadTerrianTexture(const char* path)
{
    glGenTextures(1, &heightTexture);
    glBindTexture(GL_TEXTURE_2D, heightTexture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* data = stbi_load(path,
        &width, &height, &nChannels,
        0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "Loaded heightmap of size " << height << " x " << width << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    for (int i = 0; i < rez - 1; i++) {

        for (int j = 0; j < rez - 1; j++) {
            vertices.push_back(-width / 2.0f + (i * width) / (float)rez);
            vertices.push_back(0.0f);
            vertices.push_back(-height / 2.0f + (j * height) / (float)rez);
            vertices.push_back(float(i) / float(rez));
            vertices.push_back(float(j) / float(rez));

            vertices.push_back(-width / 2.0f + ((i + 1) * width) / (float)rez);
            vertices.push_back(0.0f);
            vertices.push_back(-height / 2.0f + (j * height) / (float)rez);
            vertices.push_back(float(i + 1) / float(rez));
            vertices.push_back(float(j) / float(rez));

            vertices.push_back(-width / 2.0f + (i * width) / (float)rez);
            vertices.push_back(0.0f);
            vertices.push_back(-height / 2.0f + ((j + 1) * height) / (float)rez);
            vertices.push_back(float(i) / float(rez));
            vertices.push_back(float(j + 1) / float(rez));

            vertices.push_back(-width / 2.0f + ((i + 1) * width) / (float)rez);
            vertices.push_back(0.0f);
            vertices.push_back(-height / 2.0f + ((j + 1) * height) / (float)rez);
            vertices.push_back(float(i + 1) / float(rez));
            vertices.push_back(float(j + 1) / float(rez));

        }
    }
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texCoord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    glPatchParameteri(GL_PATCH_VERTICES, 4);
}

void Terrain::LoadTerrian(const char* path)
{
    setUpPositionsAndUV(path);
    setUpNormals();
    setUpIndices();
    buildUpVAO();

}

void Terrain::setUpNormals()
{
    std::vector<std::vector<glm::vec3>> tmpNormal[2];
    tmpNormal[0] = std::vector<std::vector<glm::vec3>>(height - 1, std::vector<glm::vec3>(width - 1, glm::vec3(0)));
    tmpNormal[1] = std::vector<std::vector<glm::vec3>>(height - 1, std::vector<glm::vec3>(width - 1, glm::vec3(0)));
    for (int i = 0; i < height - 1; i++) {
        for (int j = 0; j < width - 1; j++) {
            glm::vec3 vectorA = m_vertices[i*width+j+1].position- m_vertices[i * width + j].position;
            glm::vec3 vectorB = m_vertices[i * width + j ].position - m_vertices[(i + 1) * width + j ].position;
            tmpNormal[0][i][j] = glm::normalize(glm::cross(vectorB, vectorA));
            //std::cout << glm::to_string(tmpNormal[0][i][j]) << std::endl;

            glm::vec3 vectorC = m_vertices[(i+1) * width + j + 1].position - m_vertices[i * width + j+1].position;
            glm::vec3 vectorD = m_vertices[(i+1) * width + j].position - m_vertices[(i + 1) * width + j+1].position;
            tmpNormal[1][i][j] = glm::normalize(glm::cross(vectorC, vectorD));
            //std::cout << glm::to_string(tmpNormal[1][i][j]) << std::endl;
        }
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            bool isFirstRow = i == 0;
            bool isFirstColumn = j == 0;
            bool isLastRow = i == height - 1;
            bool isLastColumn = j == width - 1;

            glm::vec3 finalNormal = glm::vec3(0);
            //upLeft
            if (!isFirstRow && !isFirstColumn) {
                finalNormal += tmpNormal[1][i - 1][j - 1];
            }
            //upRight
            if (!isFirstRow && !isLastColumn) {
                finalNormal += tmpNormal[0][i - 1][j];
                finalNormal += tmpNormal[1][i - 1][j];
            }
            //bottomLeft
            if (!isLastRow && !isFirstColumn) {
                finalNormal += tmpNormal[0][i][j-1];
                finalNormal += tmpNormal[1][i][j-1];
            }
            //bottomRight
            if (!isLastRow && !isLastColumn) {
                finalNormal += tmpNormal[0][i][j];
            }
            m_vertices[i * width + j].normal = glm::normalize(finalNormal);
        }
    }
}

void Terrain::setUpIndices()
{
    for (unsigned i = 0; i < height - 1; i++) {
        for (unsigned j = 0; j < width - 1; j++) {
            indices.push_back((i * width) + j);
            indices.push_back((i * width) + j + 1);
            indices.push_back(((i + 1) * width) + j);

            indices.push_back((i * width) + j + 1);
            indices.push_back(((i + 1) * width) + j + 1);
            indices.push_back(((i + 1) * width) + j);
        }
    }
}

void Terrain::setUpPositionsAndUV(const char* path)
{
    unsigned char* data = stbi_load(path,
        &width, &height, &nChannels,
        0);


    if (data)
    {
        std::cout << "Loaded heightmap of size " << height << " x " << width << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    float yScale = 64.0f / 256.0f, yShift = 16.0f;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            unsigned char* texel = data + (j + width * i) * nChannels;
            unsigned char y = texel[0];
            Varying v;
            v.position = glm::vec3(-height / 2.0f + i, (int)y * yScale - yShift, -width / 2.0f + j);
            //v.position = glm::vec3(-height / 2.0f + i, 0, -width / 2.0f + j);
            v.uv = glm::vec2(float(j) / float(width), float(i) / float(height));
            m_vertices.push_back(v);
        }
    }
    stbi_image_free(data);
}

void Terrain::buildUpVAO()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Varying) * m_vertices.size(), &m_vertices.front(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices.front(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Varying), (void*)0);
    glEnableVertexAttribArray(0);
    // texCoord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Varying), (void*)offsetof(Varying, uv));
    glEnableVertexAttribArray(1);
    // normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Varying), (void*)offsetof(Varying, normal));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}
