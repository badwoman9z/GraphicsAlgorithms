#version 460 core
layout(points) in;
in VertexData {
vec3 Normal;

vec4 flux;

flat ivec3 cellIndex;

}
gs_in[];

layout(points, max_vertices = 1) out;

out FragData {
  vec3 Normal;
  vec4 flux;
}
gs_out;

void main(){

gl_Position = gl_in[0].gl_Position;

gl_PointSize = gl_in[0].gl_PointSize;

gl_Layer = gs_in[0].cellIndex.z;

gs_out.Normal = gs_in[0].Normal;
gs_out.flux = gs_in[0].flux;
EmitVertex();

EndPrimitive();



}