#version 430 core

uniform sampler2D albedo;
uniform sampler2D rsmPosition;
uniform sampler3D textureColor;
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform vec3 u_LightDir;
uniform mat4 u_LightVPMatrix;
uniform mat4 model;
uniform vec3 u_LightIntensity;
uniform vec3 cameraPos;
uniform vec3 center;
uniform float u_VoxelResolution;
uniform float VoxelGridWorldSize;



in vec2 v2f_TexCoords;

out vec4 color;
const float MAX_DIST = 25.0;
const float ALPHA_THRESH = 0.95;
float caulateShadow(vec3 worldPos){

	vec4 lightViewPos = vec4(u_LightVPMatrix*model*vec4(worldPos,1.0));

	vec3 ndc = lightViewPos.xyz/lightViewPos.w;
	ndc = ndc*0.5+0.5;

	float closetDepth = texture(rsmPosition,ndc.xy).a;

	float depth = ndc.z;

	return depth-0.005<closetDepth?1.0:0.0;


}


//vec3 caulateDirectLight(vec3 worldPos){
//
//
//	vec3 diffuse = texture(albedo,uv).rgb;
//
//	vec3 n = normalize(normal);
//
//	vec3 wi = normalize(u_LightDir);
//
//	return u_LightIntensity*max(dot(wi,n),1.0)*caulateShadow(worldPos)*diffuse;
//
//
//}
vec4 sampleVoxels(vec3 worldPos ,float lod){
	vec3 clipPos = (worldPos-center)/(VoxelGridWorldSize*0.5);

	vec3 ndc = clipPos*0.5+0.5;

	return textureLod(textureColor,ndc,lod);
	

}

vec4 coneTracing(vec3 direction , vec3 n, float tanHalfAngle ,vec3 worldPos){

	float voxelWorldSize = VoxelGridWorldSize/u_VoxelResolution;

	vec3 start = worldPos+n*voxelWorldSize;

	float d = voxelWorldSize;

	vec4 color = vec4(0);

	while(d<MAX_DIST&&color.a<ALPHA_THRESH){
		 float diameter = max(voxelWorldSize, 2.0 * tanHalfAngle * d);
		 float lodLevel = log2(diameter / voxelWorldSize);

		 vec4 voxelColor = sampleVoxels(start+direction*d,lodLevel);

		 float a = 1.0-color.a;

		 color+= a*voxelColor;
		 d += diameter*0.5 ;
		
	}
	return color;

}


void main(){

	vec3 worldPos = texture(gPosition,v2f_TexCoords).rgb;

	vec3 n = normalize(texture(gNormal,v2f_TexCoords).rgb);

	vec3 wo = normalize(cameraPos-worldPos);


	vec3 reflectDir = normalize(reflect(-wo,n));

	vec4 indirColor = coneTracing(reflectDir,n,0.07,worldPos)*10;

	//vec4 dirColor =vec4( caulateDirectLight(worldPos),1.0);



	//color = indirColor*texture(albedo,uv)+dirColor;

		    // gamma correction
    const float gamma = 2.2;
    // convert to gamma space
    color.rgb = pow(indirColor.rgb, vec3(1.0 / gamma));


}