#version 450 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;
uniform mat4 Projx;
uniform mat4 Projy;
uniform mat4 Projz;
in	 VertexData{

vec2 texCoords;

vec4 depthCoord;

vec3 Normal;


}
gs_in[];



out FragData{

vec3 Normal;

vec2 texCoords;

vec4 depthCoord;

flat int axis;
}
gs_out;


void main(){

	vec3 edge1 = gl_in[0].gl_Position.xyz-gl_in[1].gl_Position.xyz;

	vec3 edge2 = gl_in[2].gl_Position.xyz-gl_in[0].gl_Position.xyz;

	vec3 normal = abs(cross(edge1, edge2)); ; 

	if(normal.x >= normal.y && normal.x >= normal.z)
        gs_out.axis = 1;
    else if (normal.y >= normal.x  && normal.y >= normal.z)
        gs_out.axis = 2;
    else
        gs_out.axis = 3;
    mat4 projectionMatrix = gs_out.axis == 1 ? Projx : gs_out.axis == 2 ? Projy : Projz;

	for(int i=0;i<3;i++){
		gl_Position = projectionMatrix * gl_in[i].gl_Position;
		gs_out.texCoords = gs_in[i].texCoords;
		gs_out.depthCoord = gs_in[i].depthCoord;
		gs_out.Normal = gs_in[i].Normal;
		EmitVertex();
		
	}
	EndPrimitive();



}
