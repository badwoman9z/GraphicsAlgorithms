#version 430 core

layout (location = 0) in vec3 aPosition;

uniform vec3  u_Dimensions;

out VertexData{

flat ivec3 cellIndex;

}
vs_out;

uvec3 getCellPosition(uint i, uvec2 size) {
  return uvec3(i % size.x, (i / size.x) % size.y, i / (size.x * size.y));
}
void main()
{ 
	vec3 position = getCellPosition(gl_VertexID,uvec2(u_Dimensions.xy));

	vs_out.cellIndex = ivec3(position);

	vec2 ndc = (position.xy+0.5)/u_Dimensions.xy*2.0-1.0;

	gl_Position = vec4(ndc,0.0,1.0);
	
	gl_PointSize = 1.0;


}