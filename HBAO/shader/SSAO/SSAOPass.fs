#version 430 core
in vec2 TexCoord;
out vec4 color;
uniform vec3 sampler[64];
uniform sampler2D gbuffer;
uniform sampler2D u_NoiseTexture;
uniform sampler2D u_NorlamlTexture;
uniform float radius = 0.5;
uniform mat4 projection;
#define SAMPLE_NUM 64

vec3 getTexcoord(vec3 Pos){

	vec4 clipPos = projection*vec4(Pos,1.0);
	clipPos.xyz/=clipPos.w;
	clipPos.xyz = clipPos.xyz*0.5+0.5;

	return clipPos.xyz;

}
void main(){

	vec2 NoiseScale =  textureSize(gbuffer,0).xy/ 4.0;

	vec3 noise = texture(u_NoiseTexture,NoiseScale*TexCoord).rgb;
	vec3 normal = texture(u_NorlamlTexture,TexCoord).rgb;

	vec3 Tagent = normalize(noise-normal*dot(noise,normal));
	
	vec3 BiTagent = cross(normal,Tagent);

	mat3 TBN = mat3(Tagent,BiTagent,normal);

	vec3 viewPos = texture(gbuffer,TexCoord).rgb;

	float AO = 0;

	for(int i=0;i<SAMPLE_NUM;i++){
		vec3 samplePos = TBN*sampler[i];
		samplePos = viewPos+samplePos*radius;
		vec3 sampleTexPos = getTexcoord(samplePos);
		float sampleDepth = texture(gbuffer,sampleTexPos.xy).w;
		float RangeCheck = smoothstep(0.0, 1.0, radius / abs(sampleTexPos.z - sampleDepth ));
		AO += (sampleDepth<= sampleTexPos.z? 1.0:0.0)*RangeCheck;
	}
	color =  vec4(vec3(1-(AO/SAMPLE_NUM)),1.0);
	//color = vec4(viewPos,1.0);

}