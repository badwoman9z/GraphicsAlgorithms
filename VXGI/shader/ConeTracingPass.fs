#version 430 core

uniform sampler2D albedo;
uniform sampler2D rsmPosition;
uniform sampler2D specular;
uniform sampler3D textureColor;

uniform vec3 u_LightDir;
uniform mat4 u_LightVPMatrix;
uniform mat4 model;
uniform vec3 u_LightIntensity;
uniform vec3 cameraPos;
uniform vec3 center;
uniform float u_VoxelResolution;
uniform float VoxelGridWorldSize;


in vec3 worldPos;
in vec3 normal;
in vec2 uv;

out vec4 color;
const float MAX_DIST = 75.0;
const float ALPHA_THRESH = 0.95;
#define NUM_SAMPLES 50
#define BLOCKER_SEARCH_NUM_SAMPLES NUM_SAMPLES
#define PCF_NUM_SAMPLES NUM_SAMPLES
#define NUM_RINGS 10

#define EPS 1e-3
#define PI 3.141592653589793
#define PI2 6.283185307179586
float coneWeights[6] = float[](0.25, 0.15, 0.15, 0.15, 0.15, 0.15);//各锥体权重
vec3 coneDirections[6] = vec3[]
                            ( 
                            vec3(0, 0, 1),
                            vec3(0, 0.866025,0.5),
                            vec3(0.823639, 0.267617, 0.5),
                            vec3(0.509037, -0.700629, 0.5),
                            vec3(-0.509037, -0.700629, 0.5),
                            vec3(-0.823639, 0.267617, 0.5)
                            );

highp float rand_1to1(highp float x ) { 
  // -1 -1
  return fract(sin(x)*10000.0);
}

highp float rand_2to1(vec2 uv ) { 
  // 0 - 1
	const highp float a = 12.9898, b = 78.233, c = 43758.5453;
	highp float dt = dot( uv.xy, vec2( a,b ) ), sn = mod( dt, PI );
	return fract(sin(sn) * c);
}
float unpack(vec4 rgbaDepth) {
    const vec4 bitShift = vec4(1.0, 1.0/256.0, 1.0/(256.0*256.0), 1.0/(256.0*256.0*256.0));
    float depth =dot(rgbaDepth, bitShift) ;
    //shadow map 没有深度值的地方默认是0 导致的有噪点
    if(abs(depth)<EPS){
      depth=1.0;
    }

    return  depth;
}
vec2 poissonDisk[NUM_SAMPLES];

void poissonDiskSamples( const in vec2 randomSeed ) {

  float ANGLE_STEP = PI2 * float( NUM_RINGS ) / float( NUM_SAMPLES );
  float INV_NUM_SAMPLES = 1.0 / float( NUM_SAMPLES );

  float angle = rand_2to1( randomSeed ) * PI2;
  float radius = INV_NUM_SAMPLES;
  float radiusStep = radius;

  for( int i = 0; i < NUM_SAMPLES; i ++ ) {
    poissonDisk[i] = vec2( cos( angle ), sin( angle ) ) * pow( radius, 0.75 );
    radius += radiusStep;
    angle += ANGLE_STEP;
  }
}

float findBlock(vec2 uv,float depth){
    float texturesize = 4096;
    float stride = 10.0;
    float filterRange = stride/texturesize;
    poissonDiskSamples(uv);
    float avgDepth = 0.0;
    int count = 0;
    for(int i=0;i<BLOCKER_SEARCH_NUM_SAMPLES;i++){
    
        float blockDepth = unpack(texture(rsmPosition,uv+filterRange*poissonDisk[i]));
        if(depth > blockDepth-0.01){
            avgDepth += blockDepth;
            count++;
        }    
    }
    return avgDepth/float(count);

}
float PCSS(vec3 coords){
    poissonDiskSamples(coords.xy);
    float filterSize = 1.0;
    float texturesize = 4096;
    float depth = coords.z;
    float block = findBlock(coords.xy,depth);
    float penumbra = (depth - block)/block * 10.0;
    float filterRange = (penumbra*filterSize)/texturesize;
    float count = 0.0;
    for(int i = 0;i<NUM_SAMPLES;i++){
         float closeDepth = unpack(texture(rsmPosition,coords.xy+poissonDisk[i]*filterRange));
         count+= depth -0.02> closeDepth ? 0.0 : 1.0;
    
    }
    return count/float(NUM_SAMPLES);



}
float PCF(vec3 coords){
    float texturesize = textureSize(rsmPosition,0).x;
    float stride = 5.0;
    float filterRange = stride/texturesize;
    float depth = coords.z;
    float result = 0.0;
    poissonDiskSamples(coords.xy);
    for(int i=0;i<NUM_SAMPLES;i++){
       vec2 sampleUV = coords.xy + filterRange*poissonDisk[i];
       float closeDepth = unpack(texture(rsmPosition,sampleUV));
       result += depth -0.001> closeDepth ? 0.0 : 1.0;
    }
    return result/float(NUM_SAMPLES);

}




float caulateShadow(vec3 worldPos){

	vec4 lightViewPos = vec4(u_LightVPMatrix*vec4(worldPos,1.0));

	vec3 ndc = lightViewPos.xyz/lightViewPos.w;
	ndc = ndc*0.5+0.5;
    float shadow = 0.0f;
    int radius = 2;
    float current=ndc.z;
	for(int x=-radius;x<=radius;x++)		//这里用的是PCF,采样空间3x3
	{
		for(int y=-radius;y<=radius;y++)
		{
            vec2 offset = vec2(1.0/4096.0 * x, 1.0/4096.0 * y);
			float closest =unpack(texture(rsmPosition, vec2(ndc.xy+offset)));
			if(current - 0.002 > closest)
				shadow+=0.0f;
			else 
				shadow+=1.0f;
		}
	}
	shadow/=(2*radius+1)*(2*radius+1);

    
	return shadow;
    //return PCSS(ndc);


}


vec3 caulateDirectLight(vec3 worldPos){


	vec3 diffuse = texture(albedo,uv).rgb;

	vec3 n = normalize(normal);

	vec3 wi = normalize(u_LightDir);
	//u_LightIntensity*max(dot(wi,n),1.0)*caulateShadow(worldPos)*diffuse
	return u_LightIntensity*max(dot(wi,n),1.0)*diffuse;


}
vec4 sampleVoxels(vec3 worldPos ,float lod){
	vec3 clipPos = (worldPos)/(VoxelGridWorldSize*0.5);

	vec3 ndc = clipPos*0.5+0.5;

	return textureLod(textureColor,ndc,lod);
	

}

vec4 coneTracing(vec3 direction , vec3 n, float tanHalfAngle){

	float voxelWorldSize = VoxelGridWorldSize/u_VoxelResolution;

	vec3 start = worldPos+n*voxelWorldSize;

	float d = voxelWorldSize;

	    vec3 color = vec3(0);
		float alpha = 0.0;
		 float occlusion = 0.0;
	while(d<MAX_DIST&&alpha<ALPHA_THRESH){
		 float diameter = max(voxelWorldSize, 2.0 * tanHalfAngle * d);
		 float lodLevel = log2(diameter / voxelWorldSize);

		 vec4 voxelColor = sampleVoxels(start+direction*d,lodLevel);
		 occlusion += ( (1.0 - alpha) * voxelColor.a) / (1.0 + 0.03 * diameter);
		 color +=  (1.0 - alpha) * voxelColor.rgb;
		 alpha +=  (1.0 - alpha) * voxelColor.a;
		 d += diameter ;
		
	}
	return vec4(color, occlusion);

}


void main(){

	vec3 wo = normalize(cameraPos-worldPos);

	vec3 N = normalize(normal);

	vec3 reflectDir = normalize(reflect(-wo,N));

	vec4 indirColor = coneTracing(reflectDir,N,0.07);

    vec3 T;
    if(N.z != 0)
    {
        T = normalize(vec3(1,1,-(N.x + N.y) / N.z));
    }
    else if(N.y != 0)
    {
        T = normalize(vec3(1,1,-(N.x + N.z) / N.y));
    }
    else if(N.x != 0)
    {
        T = normalize(vec3(1,1,-(N.x + N.z) / N.x));
    }
    vec3 B = cross(N, T);
    mat3 TBN = mat3(T,B,N);

    vec4 diffuseReflection;

      for(int i = 0; i < 6; i++) //叠加六个锥体
     {

         diffuseReflection += coneWeights[i] * coneTracing(normalize(TBN * coneDirections[i]),N, 0.577);
     }


	vec4 dirColor =vec4( caulateDirectLight(worldPos),1.0)*caulateShadow(worldPos);

    float costheta = max(dot(reflectDir,N),0.0); 


    //color = dirColor+indirColor*texture(albedo,uv)*max(dot(reflectDir,N),0.0)+diffuseReflection*texture(albedo,uv);


	//color = diffuseReflection;

    color = dirColor+indirColor*texture(albedo,uv)*max(dot(reflectDir,N),0.0);

		    // gamma correction
    const float gamma = 2.2;
    // convert to gamma space
   color.rgb = pow(color.rgb, vec3(1.0 / gamma));


}