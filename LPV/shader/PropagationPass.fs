#version 430 core

#define PI 3.1415926f
#define SH_c0 0.282094792f
#define SH_c1 0.488602512f 
#define SH_coslobe_c0 0.886226925f
#define SH_coslobe_c1 1.02332671f

in FragData{

flat ivec3 cellIndex;

}fs_in;


layout(binding = 0) uniform sampler3D Pre_LPVGridR;
layout(binding = 1) uniform sampler3D Pre_LPVGridG;
layout(binding = 2) uniform sampler3D Pre_LPVGridB;


layout (location = 0) out vec4 LPVAccumulatorR;
layout (location = 1) out vec4 LPVAccumulatorG;
layout (location = 2) out vec4 LPVAccumulatorB;
layout (location = 3) out vec4 Next_LPVGridR;
layout (location = 4) out vec4 Next_LPVGridG;
layout (location = 5) out vec4 Next_LPVGridB;


bool isFist;
uniform vec3 u_Dimension;

const float directFaceSubtendedSolidAngle =  0.4006696846f; 
const float sideFaceSubtendedSolidAngle = 0.4234413544f; 
const float smallComponent = 0.4472135; // 1 / sqrt(5)
const float bigComponent = 0.894427; // 2 / sqrt(5)

const ivec3 propDirections[6] = {
	//+Z
	ivec3(0,0,-1),
	ivec3(0,0,1),
	//-Z

	//+X
	ivec3(-1,0,0),
	ivec3(1,0,0),
	//-X
	
	//+Y
	ivec3(0,-1,0),
	
	ivec3(0,1,0)
	//-Y
	
};

const vec3 sideDirections[6][4] = {
{vec3(0,smallComponent,bigComponent),vec3(smallComponent,0,bigComponent),vec3(0,-smallComponent,bigComponent),vec3(-smallComponent,0,bigComponent)},
{vec3(0,smallComponent,-bigComponent),vec3(smallComponent,0,-bigComponent),vec3(0,-smallComponent,-bigComponent),vec3(-smallComponent,0,-bigComponent)},
{vec3(bigComponent,0,smallComponent),vec3(bigComponent,smallComponent,0),vec3(bigComponent,0,-smallComponent),vec3(bigComponent,-smallComponent,0)},
{vec3(-bigComponent,0,smallComponent),vec3(-bigComponent,smallComponent,0),vec3(-bigComponent,0,-smallComponent),vec3(-bigComponent,-smallComponent,0)},
{vec3(0,bigComponent,smallComponent),vec3(smallComponent,bigComponent,0),vec3(0,bigComponent,-smallComponent),vec3(-smallComponent,bigComponent,0)},
{vec3(0,-bigComponent,smallComponent),vec3(smallComponent,-bigComponent,0),vec3(0,-bigComponent,-smallComponent),vec3(-smallComponent,-bigComponent,0)}
};

vec4 evalSH(vec3 dir){

return vec4(SH_c0, -SH_c1 * dir.y, SH_c1 * dir.z, -SH_c1 * dir.x);

}

vec4 evalCosLobeSH(vec3 dir){

return vec4(SH_coslobe_c0, -SH_coslobe_c1 * dir.y, SH_coslobe_c1 * dir.z, -SH_coslobe_c1 * dir.x);
	

}

vec3 caculatorTowardDir(vec3 a ,vec3 b){

 return  b-(a*(dot(a,b)));

}

void main(){

	vec4 cR = vec4(0.0);
	vec4 cG = vec4(0.0);
	vec4 cB = vec4(0.0);


	vec4 tempR = vec4(0.0);
	vec4 tempG = vec4(0.0);
	vec4 tempB = vec4(0.0);
	for(int neighbour = 0;neighbour<6;neighbour++){

		vec4 RSHcoeffsNeighbour = vec4(0.0);
		vec4 GSHcoeffsNeighbour = vec4(0.0);
		vec4 BSHcoeffsNeighbour = vec4(0.0);

		ivec3 mainDirection = propDirections[neighbour]; 

		ivec3 neighbourCellIndex = fs_in.cellIndex - mainDirection;


		vec3 direction = -vec3(float(mainDirection.x),float(mainDirection.y),float(mainDirection.z));


		RSHcoeffsNeighbour = texelFetch(Pre_LPVGridR,neighbourCellIndex,0);
		GSHcoeffsNeighbour = texelFetch(Pre_LPVGridG,neighbourCellIndex,0);
		BSHcoeffsNeighbour = texelFetch(Pre_LPVGridB,neighbourCellIndex,0);
		if(isFist){
		tempR =  RSHcoeffsNeighbour;
		tempG =  GSHcoeffsNeighbour;
		tempB =  BSHcoeffsNeighbour;
		
		}


		float direct_face_R_flux = max(dot(RSHcoeffsNeighbour,evalSH(direction)),0.0)*directFaceSubtendedSolidAngle;
		float direct_face_G_flux = max(dot(GSHcoeffsNeighbour,evalSH(direction)),0.0)*directFaceSubtendedSolidAngle;
		float direct_face_B_flux = max(dot(BSHcoeffsNeighbour,evalSH(direction)),0.0)*directFaceSubtendedSolidAngle;

		cR += (direct_face_R_flux/PI)*evalCosLobeSH(-direction);
		cG += (direct_face_G_flux/PI)*evalCosLobeSH(-direction);
		cB += (direct_face_B_flux/PI)*evalCosLobeSH(-direction);

		for(int face = 0;face<4;face++){
			vec3 side_direction = sideDirections[neighbour][face];

			float side_face_R_flux = max(dot(RSHcoeffsNeighbour,evalSH(side_direction)),0.0)*sideFaceSubtendedSolidAngle;
			float side_face_G_flux = max(dot(GSHcoeffsNeighbour,evalSH(side_direction)),0.0)*sideFaceSubtendedSolidAngle;
			float side_face_B_flux = max(dot(BSHcoeffsNeighbour,evalSH(side_direction)),0.0)*sideFaceSubtendedSolidAngle;

			vec3 side_normal = caculatorTowardDir(direction,side_direction);

			cR+=(side_face_R_flux/PI)*evalCosLobeSH(side_normal);
			cG+=(side_face_G_flux/PI)*evalCosLobeSH(side_normal);
			cB+=(side_face_B_flux/PI)*evalCosLobeSH(side_normal);
	
		}


	}

	if(isFist){
	
		LPVAccumulatorR = tempR+cR;
		LPVAccumulatorG = tempG+cG;
		LPVAccumulatorB = tempB+cB;
//	LPVAccumulatorR = vec4(1.0);
//	LPVAccumulatorG = vec4(1.0);
//	LPVAccumulatorB = vec4(1.0);
	
	}else{
		LPVAccumulatorR = cR;
		LPVAccumulatorG = cG;
		LPVAccumulatorB = cB;
//	LPVAccumulatorR = vec4(1.0);
//	LPVAccumulatorG = vec4(1.0);
//	LPVAccumulatorB = vec4(1.0);
	
	}

		Next_LPVGridR = cR;
		Next_LPVGridG = cG;
		Next_LPVGridB = cB;

//	Next_LPVGridR = vec4(1.0);
//	Next_LPVGridG = vec4(1.0);
//	Next_LPVGridB = vec4(1.0);


}
