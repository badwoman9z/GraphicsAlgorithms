#version 430 core

layout (location = 0) in vec2 aPosition;
uniform sampler2D rsmNormal;
uniform sampler2D rsmPosition;



uniform float u_CellSize;
uniform vec3 u_MinAABB;
uniform int u_RSMResolution;
uniform vec3 u_Dimensions;
uniform mat4 view;
uniform mat4 projection;


out VertexData {
  vec3 cellCoord;
  vec3 N;
}
vs_out;
uvec3 unflatten3D(uint i, uvec2 size) {
  return uvec3(i % size.x, (i / size.x) % size.y, i / (size.x * size.y));
}
uvec2 unflatten2D(uint i, uint width) { return uvec2(i % width, i / width); }

void main() {
  const ivec3 id =
    ivec3(unflatten3D(gl_VertexID, uvec2(u_Dimensions.xy))); // [0..GridSize]

  const ivec2 coord = ivec2(unflatten2D(gl_VertexID, u_RSMResolution));
  const vec3 position = texelFetch(rsmPosition, coord.xy, 0).rgb;
  const vec3 N = texelFetch(rsmNormal, coord.xy, 0).rgb;

  vs_out.cellCoord = (position - u_MinAABB) / u_CellSize /u_Dimensions;
  vs_out.N = N;

  gl_Position = projection*view * vec4(position + 0.5 * N, 1.0);

  gl_PointSize = 100.0;
}

