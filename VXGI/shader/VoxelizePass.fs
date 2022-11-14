#version 450 core
#define MAX_COLOR_VALUES 256.0
//layout(RGBA8,binding = 0) uniform image3D u_VoxelGrid;
layout(binding = 0, r32ui) uniform volatile coherent uimage3D u_VoxelGrid;
uniform sampler2D albedo;
uniform vec3 u_LightDir;
uniform sampler2D rsmPosition;
uniform int u_VoxelResolution;


#define NUM_SAMPLES 50
#define BLOCKER_SEARCH_NUM_SAMPLES NUM_SAMPLES
#define PCF_NUM_SAMPLES NUM_SAMPLES
#define NUM_RINGS 10

#define EPS 1e-3
#define PI 3.141592653589793
#define PI2 6.283185307179586


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




void imageAtomicRGBA8Avg( uimage3D imgUI, ivec3 coords, vec4 val)
{
	uint newVal = packUnorm4x8(val);
	uint prevStoredVal = 0;
	uint curStoredVal;
	// Loop as long as destination value gets changed by other threads
	while ((curStoredVal = imageAtomicCompSwap(imgUI, coords, prevStoredVal, newVal)) != prevStoredVal)
	{
		prevStoredVal = curStoredVal;
		vec4 rval = unpackUnorm4x8(curStoredVal);
		rval.w *= MAX_COLOR_VALUES;
		rval.xyz = (rval.xyz * rval.w); // Denormalize
		vec4 curValF = rval + val; // Add new value
		curValF.xyz /= (curValF.w); // Renormalize
		curValF.w /= MAX_COLOR_VALUES;
		newVal = packUnorm4x8(curValF);
	}
}
in FragData {
vec3 Normal;

vec2 texCoords;

vec4 depthCoord;

flat int axis;

}
fs_in;
float caulateShadow(vec3 shadowPos){


    float shadow = 0.0f;
    int radius = 2;
    float current=shadowPos.z;
	for(int x=-radius;x<=radius;x++)		//这里用的是PCF,采样空间3x3
	{
		for(int y=-radius;y<=radius;y++)
		{
            vec2 offset = vec2(1.0/4096.0 * x, 1.0/4096.0 * y);
			float closest =unpack(texture(rsmPosition, vec2(shadowPos.xy+offset)));
			if(current - 0.002 > closest)
				shadow+=0.0f;
			else 
				shadow+=1.0f;
		}
	}
	shadow/=(2*radius+1)*(2*radius+1);

    
	return shadow;



}

void main(){


	vec4 diffuse = texture(albedo,fs_in.texCoords);

    vec3 shadowPos = fs_in.depthCoord.xyz/fs_in.depthCoord.w;
    shadowPos  = shadowPos * 0.5 +  0.5;
    
    
    float v = caulateShadow( shadowPos );

	ivec3 camPos = ivec3(gl_FragCoord.x, gl_FragCoord.y, u_VoxelResolution * gl_FragCoord.z);
	ivec3 voxelPos;
	if(fs_in.axis == 1) 
	{

	    voxelPos.x = u_VoxelResolution -1 - camPos.z;
		voxelPos.z = u_VoxelResolution -1 -camPos.x;
		voxelPos.y = camPos.y;
	}
	else if(fs_in.axis == 2) 
	{

	    voxelPos.z = u_VoxelResolution -1 -camPos.y;
		voxelPos.y = u_VoxelResolution -1 - camPos.z;
		voxelPos.x = camPos.x;
        
	} 
	else 
	{
	   	voxelPos = camPos;
		voxelPos.z = u_VoxelResolution -1 -camPos.z;
		
	}
	vec4 color = diffuse*v;

//imageStore(u_VoxelGrid,voxelPos,vec4(color.rgb,1.0));
imageAtomicRGBA8Avg(u_VoxelGrid,voxelPos,vec4(color.rgb,1.0));



}