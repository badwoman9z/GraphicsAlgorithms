
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

float rand(vec2 uvs)
{
	return fract(sin(dot(uvs, vec2(12.9898, 78.233))) * 43758.5453);
}
vec3 getTexelPos(vec3 Pos){
	vec4 clipPos = projection*vec4(Pos,1.0);

	clipPos = clipPos/clipPos.w;

	clipPos.xyz = clipPos.xyz*0.5+0.5;
	
	return clipPos.xyz;

}
float caculateTangent2Sin(float tangent){

	return tangent/sqrt(1+(tangent*tangent));

}
float Fall0f(vec3 Hpos,vec3 viewPos){

	return 1- (length(Hpos-viewPos)*length(Hpos-viewPos))/(radius*radius);
}
vec2 RotateDirections(vec2 Dir, vec2 CosSin)
{
    return vec2(Dir.x*CosSin.x - Dir.y*CosSin.y,
                  Dir.x*CosSin.y + Dir.y*CosSin.x);
}
float caculateDirectionAO(vec2 rotation,float stepLength,vec3 viewPos,float tangentT){
	//stepLength = 1;
	//float stepl = 1.0;
	float maxTangentT = tangentT+0.5;
	float AO;
	for(int i=1;i<=NumSamples;i++){
	
		vec2 deltaUV = stepLength*i*vec2(rotation.x,rotation.y);
		//vec2 deltaUV = stepl*i*vec2(rotation.x,rotation.y)/textureSize(gbuffer,0).xy;
		vec3 HPos = texture(gbuffer,deltaUV+TexCoord).rgb;

		vec3 Hvector = HPos-viewPos;

		float dist = length(Hvector);
		float tangentH = Hvector.z/length(Hvector.xy);

		if(dist<radius&&tangentH>maxTangentT){
		
			AO += (caculateTangent2Sin(tangentH)-caculateTangent2Sin(tangentT))*Fall0f(HPos,viewPos);
			maxTangentT = tangentH;
		
		}

	
	
	}
	return AO;


}

void main(void)
{
	
	vec3 viewPos = texture(gbuffer,TexCoord).rgb;

	vec3 endPos = viewPos+vec3(radius,0,0);

	vec3 texelPos = getTexelPos(viewPos);
	
	vec3 texelEndPos = getTexelPos(endPos);
	float stepLength = radius*u_FOV/-viewPos.z;

	//float stepLength = length(texelEndPos-texelPos)/NumSamples;

	vec2 NoiseScale =  textureSize(gbuffer,0).xy/ 4.0;

	vec2 random = texture(u_NoiseTexture,TexCoord*NoiseScale).rg;
	float startAngle = 0;

	float angleStep = 2*PI/NumDirections;

	vec3 dx = normalize(dFdx(viewPos));
	vec3 dy = normalize(dFdy(viewPos));
	float AO = 0.0f;
	for(int i=0;i<NumDirections;i++){
	
		float angle = startAngle+i*angleStep;

		//vec2 rotation = vec2(cos(angle),sin(angle));
		vec2 dir = RotateDirections(vec2(cos(angle),sin(angle)),random);
		vec3 tVector = dx*dir.x+dy*dir.y;

		float tangentT = tVector.z/(length(tVector.xy));
		
		AO += caculateDirectionAO(dir,stepLength,viewPos,tangentT);
	
	}

	Color_ = vec4(vec3(1-(AO/NumDirections)),1.0);
	//Color_ = vec4(vec3(stepLength),1.0);
	//vec2 uv = gl_FragCoord.xy/textureSize(gbuffer,0).xy;
	//Color_ = vec4(texture(gbuffer,uv).rgb,1.0);

}