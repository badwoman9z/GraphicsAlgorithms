#version 450 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
uniform vec2 offsets[12];
uniform mat4 model;



void main()
{   
	vec2 offset = offsets[gl_InstanceID];
	vec4 worldPos = model * vec4(aPosition, 1.0f);
	worldPos.z+=offset.y;
	gl_Position =  worldPos;
}