#include"Terrain.h"
#include "stb_image.h"
Terrain::Terrain(const char* path)
{
    unsigned char* data = stbi_load(path,
        &width, &height, &nChannels,
        0);
    float yScale = 64.0f / 256.0f, yShift = 16.0f;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // retrieve texel for (i,j) tex coord
            unsigned char* texel = data + (j + width * i) * nChannels;
            // raw height at coordinate
            unsigned char y = texel[0];
            vertices.push_back(-height / 2 + i);
            vertices.push_back(int(y) * yScale - yShift);
            vertices.push_back(-width / 2 + j);
        }
    }
    for (int i = 0; i < height - 1; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < 2; k++) {
                indices.push_back(j + width * (i + k));
            }
        }
    }
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),       // size of vertices buffer
        &vertices[0],                          // pointer to first element
        GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(unsigned int), // size of indices buffer
        &indices[0],                           // pointer to first element
        GL_STATIC_DRAW);
    glBindVertexArray(0);
    NUM_STRIPS = height - 1;
    NUM_VERTS_PER_STRIP = width * 2;
}

void Terrain::Draw()
{
    glBindVertexArray(VAO);
    for (unsigned i = 0; i < NUM_STRIPS; i++) {
        glDrawElements(GL_TRIANGLE_STRIP, NUM_VERTS_PER_STRIP, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * NUM_VERTS_PER_STRIP * i));
    }
    glBindVertexArray(0);
}
