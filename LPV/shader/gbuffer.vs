#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 textureCoords;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 u_LightVPMatrix;

out vec3 worldPos;
out vec3 normal;
out vec2 texCoord;
out vec4 viewPos;


void main()
{   

    normal =transpose(inverse(mat3(model))) * aNormal;
    vec4 world = model*vec4( aPos, 1.0);
    worldPos = world.xyz;
    texCoord = textureCoords;

    viewPos = u_LightVPMatrix*world;
    gl_Position = projection*view*model*vec4( aPos, 1.0);
    

}