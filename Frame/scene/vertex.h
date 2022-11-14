#pragma once
#include "glm/glm.hpp"

#define MAX_BONE_INFLUENCE 4
struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Varying
{
    glm::vec4 position;
    glm::vec2 uv;
    glm::vec3 normal;
    glm::mat3 TBN;
};

struct SkyboxVarying
{
    glm::vec4 position;
    glm::vec3 uv;
};
