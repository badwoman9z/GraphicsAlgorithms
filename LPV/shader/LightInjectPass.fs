#version 460 core


layout (location = 0) out vec4 LPVGridR;
layout (location = 1) out vec4 LPVGridG;
layout (location = 2) out vec4 LPVGridB;

#define PI 3.1415926f

#define SH_coslobe_c0 0.886226925f
#define SH_coslobe_c1 1.02332671f


in FragData {
  vec3 Normal;
  vec4 flux;
}
fs_in;



uniform float u_CellSize;




vec4 evalSH(vec3 normal){
 
 return vec4(SH_coslobe_c0 , -SH_coslobe_c1 * normal.y , SH_coslobe_c1 * normal.z , -SH_coslobe_c1 * normal.x );
}

void main(){

 // if(length(fs_in.Normal)<0.01) discard;
  
  LPVGridR = (fs_in.flux.r/PI)*evalSH(fs_in.Normal);
  LPVGridG = (fs_in.flux.g/PI)*evalSH(fs_in.Normal);
  LPVGridB = (fs_in.flux.b/PI)*evalSH(fs_in.Normal);



}
