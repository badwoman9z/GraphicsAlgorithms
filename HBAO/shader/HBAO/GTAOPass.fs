#version 430 core
const float PI = 3.14159265;

in vec2 TexCoord;

out vec4 Color_;

uniform mat4 projection;
uniform sampler2D gbuffer;
uniform sampler2D u_NoiseTexture;
uniform int NumDirections = 4;
uniform int NumSamples = 4;
uniform float radius =0.05f;
uniform float windowHeight =1080;
uniform float u_FOV;
void main(){

	


}