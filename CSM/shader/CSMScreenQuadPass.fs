#version 460 core
uniform sampler2D texture_diffuse1;
uniform sampler2DArray rsmFlux;
uniform mat4 view;

uniform float farPlanes[4];
uniform mat4 u_lightProjection[4];
uniform vec3 lightDir;
in vec4 world;
in vec2 uv;
out vec4 color;
in vec3 normal;
int cascadedLevel = 0;
float BilinearPCF(vec3 ndcPos , float depth, int cascadedLevel){
		float result = 0.0f;
		int filterSize = 3;
		vec2 texelSize = 1.0 / vec2(textureSize(rsmFlux, 0));
		for(int i = -filterSize; i<=filterSize;i++){
		
			for(int j= -filterSize;j<=filterSize;j++){
			
				vec2 uvOffset = vec2(i, j) * texelSize;

				vec2 uv = ndcPos.xy+uvOffset;

				vec2 samplePos = floor(uv* vec2(textureSize(rsmFlux, 0)));

				vec2 fraction = fract(uv* vec2(textureSize(rsmFlux, 0)));

				vec4 closetDepth = textureGather(rsmFlux,vec3(samplePos/vec2(textureSize(rsmFlux, 0)),cascadedLevel),0);
				
				vec4 shadow = step(vec4(depth),closetDepth+0.005);

				        float d1 = mix(shadow.w, shadow.z, fraction.x);
						float d2 = mix(shadow.x, shadow.y, fraction.x);
						float d = mix(d1, d2, fraction.y);
						result += d;

						
			}
		
		
		}
		float sum = ((filterSize * 2 + 1) * (filterSize * 2 + 1));
		return result / sum;
}
void caculateTent3x3Weights(float offset,out vec4 weights){

	float area01 = (offset+0.5)*(offset+0.5)*0.5;

	weights.w = area01;

	weights.x = area01-offset;

	float leftArea = 1-offset;
	
	float clampOffsetLeft = min(offset,0);

	weights.y = leftArea - (clampOffsetLeft*clampOffsetLeft);

	float rightArea = 1+offset;

	float clampOffsetRight = max(offset,0);

	weights.z = rightArea - (clampOffsetRight*clampOffsetRight);

}
float caculateTent3x3Group(vec2 fragPos, vec2 weightsU, vec2 weightsV ,float depth){

	vec2 uv = (fragPos-0.5) / vec2(textureSize(rsmFlux, 0));

	vec4 closetDepth = textureGather(rsmFlux,vec3(uv,cascadedLevel),0);

	vec4 shadow = step(vec4(depth),closetDepth+0.005);

	vec2 fraction;

	fraction.x = weightsU.y/(weightsU.x+weightsU.y);

	fraction.y = weightsV.y/(weightsV.x+weightsV.y);

	float d1 = mix(shadow.w, shadow.z, fraction.x);
	
	float d2 = mix(shadow.x, shadow.y, fraction.x);
	
	float d = mix(d1, d2, fraction.y);
	
	return d;
}
vec4 caculateTent3x3GroupWeights(vec4 weightsU , vec4 weightsV){

    return vec4(dot(weightsU.xyxy,weightsV.xxyy), 
				dot(weightsU.zwzw,weightsV.xxyy),
				dot(weightsU.xyxy,weightsV.zzww),
				dot(weightsU.zwzw,weightsV.zzww)) ;

}
float tentPCF3X3(vec3 ndcPos){

	vec2 fragCoords = ndcPos.xy* vec2(textureSize(rsmFlux, 0));

	vec2 samplePos = floor(fragCoords+0.5);

	vec2 offset = fragCoords - samplePos;

	vec4 weightsU,weightsV;

	caculateTent3x3Weights(offset.x,weightsU);
	
	caculateTent3x3Weights(offset.y,weightsV);

	weightsU*=0.4444444;
	weightsV*=0.4444444;
	
	vec4 groupWeights = caculateTent3x3GroupWeights(weightsU,weightsV);

	vec4 groupShadow;

	groupShadow.x = caculateTent3x3Group(samplePos+vec2(-1,-1),weightsU.xy,weightsV.xy,ndcPos.z);

	groupShadow.y = caculateTent3x3Group(samplePos+vec2(1,-1),weightsU.zw,weightsV.xy,ndcPos.z);

	groupShadow.z = caculateTent3x3Group(samplePos+vec2(-1,1),weightsU.xy,weightsV.zw,ndcPos.z);

	groupShadow.w = caculateTent3x3Group(samplePos+vec2(1,1),weightsU.zw,weightsV.zw,ndcPos.z);

	return dot(groupShadow,groupWeights);
}
float sampleShadow(vec3 ndcPos){

	float closetDepth = texture(rsmFlux,vec3(ndcPos.xy,cascadedLevel)).r;
	float depth = ndcPos.z;
	return step(depth,closetDepth+0.005);
}
float PCFShadow(vec3 ndcPos){

//	    // PCF
    float shadow = 0.0;
	float depth = ndcPos.z;
    vec2 texelSize = 1.0 / vec2(textureSize(rsmFlux, 0));
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(rsmFlux, vec3(ndcPos.xy + vec2(x, y) * texelSize, cascadedLevel)).r;
            shadow += step(depth,pcfDepth+0.005);        
        }    
    }
	 shadow /= 9.0;
	 return shadow;
}
float caculateShadow(vec4 world){

	vec4 viewWorldPos = view*world;
	for(int i=0;i<4;i++){
		
		if(-viewWorldPos.z>farPlanes[i]){
			cascadedLevel = i+1;
			//break;
		}
	}

	vec4 clipPos = u_lightProjection[cascadedLevel]*world;

	clipPos/=clipPos.w;

	vec3 ndcPos = clipPos.xyz*0.5+0.5;
	//ndcPos = ndcPos*0.5+0.5;

	float closetDepth = texture(rsmFlux,vec3(ndcPos.xy,cascadedLevel)).r;

	float depth = ndcPos.z;



//	    // PCF
//    float shadow = 0.0;
//    vec2 texelSize = 1.0 / vec2(textureSize(rsmFlux, 0));
//    for(int x = -3; x <= 3; ++x)
//    {
//        for(int y = -3; y <= 3; ++y)
//        {
//            float pcfDepth = texture(rsmFlux, vec3(ndcPos.xy + vec2(x, y) * texelSize, cascadedLevel)).r;
//            shadow += tentPCF3X3(vec3(ndcPos.xy + vec2(x, y) * texelSize,ndcPos.z));        
//        }    
//    }
//	 shadow /= 49.0;

	//return BilinearPCF(ndcPos,depth,cascadedLevel);
	return tentPCF3X3(ndcPos);
	//return sampleShadow(ndcPos);
	//return PCFShadow(ndcPos);
	//return shadow;
}



float caculate(vec4 world){

	vec4 viewWorldPos = view*world;
	for(int i=0;i<4;i++){
		
		if(-viewWorldPos.z>farPlanes[i]){
			cascadedLevel = i+1;
			//break;
		}
	}

	vec4 clipPos = u_lightProjection[cascadedLevel]*world;

	clipPos/=clipPos.w;

	vec3 ndcPos = clipPos.xyz*0.5+0.5;
	//ndcPos = ndcPos*0.5+0.5;

	float closetDepth = texture(rsmFlux,vec3(ndcPos.xy,cascadedLevel)).r;


	return closetDepth ;



}

void main(){
	float shadow = caculateShadow(world);
	switch(cascadedLevel){
		case 0:
			color = vec4(1.0,0.0,0.0,1.0)*vec4(texture(texture_diffuse1,uv))*shadow;
			break;															
		case 1:																
			color = vec4(0.0,1.0,0.0,1.0)*vec4(texture(texture_diffuse1,uv))*shadow;
			break;															
		case 2:																
			color = vec4(0.0,0.0,1.0,1.0)*vec4(texture(texture_diffuse1,uv))*shadow;
			break;
		case 3:
			color = texture(texture_diffuse1,uv)*shadow;
			break;

	}
	//color = vec4(vec3(cascadedLevel/4.0f),1.0);
	//color = texture(texture_diffuse1,uv)*shadow+0.1*texture(texture_diffuse1,uv);
	//color = vec4(caculate(world));
	//color = vec4(vec3(shadow),1.0);
	
}