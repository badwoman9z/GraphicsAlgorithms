#version 430 core
#extension GL_NV_shader_atomic_float : require
#extension GL_NV_shader_atomic_fp16_vector : require
#extension GL_NV_gpu_shader5 : require

#define SH_coslobe_c0 0.886226925f
#define SH_coslobe_c1 1.02332671f

layout(rgba16f ,binding = 0) uniform image3D GeometryInjection;

flat in ivec3 v2f_cellIndex;
in vec3 v2f_rsmPosition;
in vec3 v2f_rsm_Normal;
in float surfelArea;
uniform float u_CellSize;
uniform vec3 u_LightDir;
vec4 evalCosineLobeToDir(vec3 dir) {
	return vec4(SH_coslobe_c0, -SH_coslobe_c1 * dir.y, SH_coslobe_c1 * dir.z, -SH_coslobe_c1 * dir.x);
}
float BlockingPotencial(){

	return surfelArea/(u_CellSize*u_CellSize);

}
void main(){

	float Blocking = BlockingPotencial();

	vec4 SH_Geometry = Blocking*evalCosineLobeToDir(v2f_rsm_Normal);

	imageAtomicAdd(GeometryInjection,v2f_cellIndex,f16vec4(SH_Geometry));

}