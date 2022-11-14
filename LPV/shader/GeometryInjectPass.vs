#version 430 core

layout (location = 0) in vec2 _Position;
uniform sampler2D rsmNormal;
uniform sampler2D rsmPosition;

flat out ivec3 v2f_cellIndex;
out vec3 v2f_rsmPosition;
out vec3 v2f_rsm_Normal;
out float surfelArea;

uniform float u_CellSize;
uniform vec3 u_MinAABB;
uniform int u_RSMResolution;
uniform vec3 u_LightDir;
uniform vec3 u_LightVPMatrix;

ivec3 getCellIndex( vec3 Pos){

return ivec3((Pos - u_MinAABB) / u_CellSize);
}

void main(){

	ivec2 RSMCoords = ivec2(gl_VertexID % u_RSMResolution, gl_VertexID / u_RSMResolution);

	v2f_rsmPosition = texelFetch(rsmPosition,RSMCoords,0).rgb;
	v2f_rsm_Normal = texelFetch(rsmNormal,RSMCoords,0).rgb;
	surfelArea = float(4.0)/(u_RSMResolution*u_RSMResolution);
	v2f_cellIndex = getCellIndex(v2f_rsmPosition);





}