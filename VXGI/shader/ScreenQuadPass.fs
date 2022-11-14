#version 430 core
#define SH_c0 0.282094792f // 1 / 2sqrt(pi)
#define SH_c1 0.488602512f // sqrt(3/pi) / 2
#define PI 3.1415926f
in  vec2 v2f_TexCoords;
out vec4 Color_;

uniform sampler2D rsmFlux;





void main()		  
{
	Color_ = texture(rsmFlux,v2f_TexCoords);
}