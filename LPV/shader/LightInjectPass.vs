#version 430 core

layout (location = 0) in vec2 aPosition;
uniform sampler2D rsmFlux;
uniform sampler2D rsmNormal;
uniform sampler2D rsmPosition;

uniform float u_CellSize;
uniform vec3 u_MinAABB;
uniform int u_RSMResolution;
uniform vec3 u_Dimensions;

out VertexData{
vec3 Normal;

vec4 flux;

flat ivec3 cellIndex;


}
vs_out;
ivec3 getCellIndx(vec3 pos,vec3 N){

	return ivec3((pos-u_MinAABB)/u_CellSize+ 0.5 * N);

}
void main(){

	ivec2 RSMCoords = ivec2(gl_VertexID%u_RSMResolution,gl_VertexID/u_RSMResolution);

	vec3 worldPos = texelFetch(rsmPosition,RSMCoords,0).rgb;

	vec3 worldNormal = texelFetch(rsmNormal,RSMCoords,0).xyz;

	vec4 flux = texelFetch(rsmFlux,RSMCoords,0);

	ivec3 cellIndex = getCellIndx(worldPos,worldNormal);

	vs_out.Normal = worldNormal;
	vs_out.flux = flux;
	vs_out.cellIndex = cellIndex;


	vec2 ndc = (vec2(cellIndex.xy)+0.5)/u_Dimensions.xy*2.0-1.0;
	gl_Position = vec4(ndc,0.0,1.0);
	gl_PointSize = 1.0;



}