#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 textureCoords;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform sampler2DArray rsmFlux;
uniform float farPlanes[4];
uniform mat4 u_lightProjection[4];
uniform vec2 offsets[12];
out vec4 world;
out vec2 uv;
//out vec4 test;
//out float t;
out vec3 normal;
void main()
{   
    
    
    vec2 offset = offsets[gl_InstanceID];
	world =model*vec4( aPos, 1.0);
	world.z+=offset.y;
     
     

    normal = aNormal;
     uv = textureCoords;
    gl_Position = projection*view*world;
    

}