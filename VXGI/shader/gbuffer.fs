#version 330 core
layout (location = 0) out vec4 Albedo;
layout (location = 1) out vec3 Normal;
layout (location = 2) out vec4 Position;

uniform sampler2D albedo;
uniform sampler2D specular;
uniform sampler2D rsmPosition;
uniform mat4 u_LightVPMatrix;

uniform vec3 u_LightDir;


in vec3 worldPos;
in vec3 normal;
in vec2 texCoord;
in vec4 viewPos;

#define NUM_SAMPLES 50
#define BLOCKER_SEARCH_NUM_SAMPLES NUM_SAMPLES
#define PCF_NUM_SAMPLES NUM_SAMPLES
#define NUM_RINGS 10

#define EPS 1e-3
#define PI 3.141592653589793
#define PI2 6.283185307179586

uniform float u_Near =  0.1f;
uniform float u_Far = 1000.0f;
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
    float texturesize = 400;
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
    float texturesize = 400;
    float depth = coords.z;
    float block = findBlock(coords.xy,depth);
    float penumbra = (depth - block)/block * 10.0;
    float filterRange = (penumbra*filterSize)/texturesize;
    float count = 0.0;
    for(int i = 0;i<NUM_SAMPLES;i++){
         float closeDepth = unpack(texture(rsmPosition,coords.xy+poissonDisk[i]*filterRange));
         count+= depth -0.01> closeDepth ? 0.0 : 1.0;
    
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
float shadowMap(vec3 shadowPos ){
    float realDeth = shadowPos.z;
    float shadowDeth = unpack(texture2D(rsmPosition ,shadowPos.xy));
    if(realDeth>shadowDeth){
        return 0.0;
    }
    return 1.0;
}

void main()
{   
    Position = vec4(worldPos,gl_FragCoord.z);

   vec4 lightViewPos =  u_LightVPMatrix*vec4(worldPos,1.0);

   vec3 shadowPos = lightViewPos.xyz/lightViewPos.w;
   shadowPos  = shadowPos * 0.5 +  0.5;
   float v = PCF( shadowPos );
   Normal =normalize(normal);

   Albedo = texture2D(albedo,texCoord)*v*max(dot(Normal,normalize(u_LightDir)),1.0);
    
}


























