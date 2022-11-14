#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 u_LightVPMatrix;

out vec2 TexCoords;
out vec3 WorldNormal;
out vec3 WorldPos;

void main()
{   

	vec4 FragPosInWorldSpace = model * vec4(aPosition, 1.0f);
	TexCoords = aTexCoord;
	WorldNormal = aNormal;	
	WorldPos = vec3(FragPosInWorldSpace);
	gl_Position = u_LightVPMatrix * FragPosInWorldSpace;
}