#version 450 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;

uniform mat4 u_LightVPMatrix;

out VertexData{

vec2 texCoords;

vec4 depthCoord;

vec3 Normal;


}
vs_out;


void main(){

	gl_Position = model*vec4(aPosition,1.0);

	vs_out.texCoords = aTexCoord;

	vs_out.depthCoord = u_LightVPMatrix*model*vec4(aPosition,1.0);


	vs_out.Normal = normalize(aNormal);



}