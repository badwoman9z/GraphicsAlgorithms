#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 textureCoords;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 worldPos;
out vec3 normal;
out vec2 uv;

void main(){

    normal =transpose(inverse(mat3(model))) * aNormal;
    vec4 world = model*vec4( aPos, 1.0);
    worldPos = world.xyz;
    uv= textureCoords;
    gl_Position = projection*view*model*vec4( aPos, 1.0);


}