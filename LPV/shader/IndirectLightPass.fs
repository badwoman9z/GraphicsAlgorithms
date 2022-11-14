#version 430 core
#define SH_c0 0.282094792f // 1 / 2sqrt(pi)
#define SH_c1 0.488602512f // sqrt(3/pi) / 2
#define PI 3.1415926f
in  vec2 v2f_TexCoords;
out vec4 Color_;

uniform sampler2D normal;
uniform sampler2D position;
uniform sampler2D gbufferColor;
uniform sampler3D LPVAccumulatorR;
uniform sampler3D LPVAccumulatorG;
uniform sampler3D LPVAccumulatorB;
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
	

	ivec3 CellIndex = getCellIndex(wordPos);


	vec3 BasePos = vec3(CellIndex)*u_CellSize+u_MinAABB;

	vec3 LpvIntensity  = vec3(0);
	vec3 alpha = clamp((wordPos -BasePos) / u_CellSize, vec3(0), vec3(1));
		
    vec4 SHintensity = evalSH(Normal);
	for (int i = 0; i < 8; ++i) {
		ivec3 offset = ivec3(i, i >> 1, i >> 2) & ivec3(1);
		ivec3 LpvCellCoords = getCellIndex(wordPos) + offset;
		vec3 trilinear = mix (1 - alpha, alpha, offset);
		float weight = trilinear.x * trilinear.y * trilinear.z;

		weight = max(0.0002, weight);
		LpvIntensity += weight 
		* vec3( 
				dot(SHintensity, texelFetch(LPVAccumulatorR, LpvCellCoords,0)),
				dot(SHintensity, texelFetch(LPVAccumulatorG, LpvCellCoords,0)),
				dot(SHintensity, texelFetch(LPVAccumulatorB, LpvCellCoords,0))
		);
	}
	Color_ =vec4(LpvIntensity*texture(gbufferColor,v2f_TexCoords).rgb,1.0);

}