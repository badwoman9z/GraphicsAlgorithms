#version 450 core
layout (triangles, invocations = 4) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 u_LightVPMatrix[4];

void main(){


	for(int i=0;i<3;i++){
		gl_Position = u_LightVPMatrix[gl_InvocationID]*gl_in[i].gl_Position;
		gl_Layer = gl_InvocationID;
		EmitVertex();
	
	}
	EndPrimitive();

}