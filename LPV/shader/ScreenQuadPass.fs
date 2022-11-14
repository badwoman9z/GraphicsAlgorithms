#version 430 core
#define SH_c0 0.282094792f // 1 / 2sqrt(pi)
#define SH_c1 0.488602512f // sqrt(3/pi) / 2
#define PI 3.1415926f
in  vec2 v2f_TexCoords;
out vec4 Color_;

uniform sampler2D directColor;
uniform sampler2D normal;
uniform sampler2D position;
uniform sampler2D gbufferColor;
uniform sampler3D LPVAccumulatorR;
uniform sampler3D LPVAccumulatorG;
uniform sampler3D LPVAccumulatorB;

uniform sampler2D indirectLight;
uniform float u_CellSize;
uniform vec3 u_MinAABB;

vec4 evalSH(vec3 dir){

return vec4( SH_c0, -SH_c1 * dir.y, SH_c1 * dir.z, -SH_c1 * dir.x );
}

ivec3 getCellIndex(vec3 Pos){


return ivec3((Pos - u_MinAABB) / u_CellSize);


}


void main()		  
{
	vec3 Normal = texture(normal,v2f_TexCoords).xyz;
	vec3 wordPos = texture(position,v2f_TexCoords).xyz;
	
	vec3 dir = texture(directColor,v2f_TexCoords).rgb;
	vec3 indir = texture(indirectLight,v2f_TexCoords).rgb;
	vec3 c= (indir/50+dir);
	//vec3 c = dir;
	//vec3 c= dir*0.8f+(5*indir);
	vec3 mapped = vec3(1.0) - exp(-c*2.0f);
	mapped = pow(mapped,vec3(1.0f/2.2f));
	
	Color_ =vec4(c,1.0);
	//Color_ = test*10;
	//Color_ =vec4(test*10);
}