#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 u_LightVPMatrix;



void main()
{   

	vec4 FragPosInWorldSpace = model * vec4(aPosition, 1.0f);
	gl_Position = u_LightVPMatrix * FragPosInWorldSpace;
}