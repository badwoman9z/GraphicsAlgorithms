#version 450 core
//layout (location = 0) out vec3 Position;
in vec3 f_world_pos;
out vec4 color;
void main()
{
	 color = vec4(f_world_pos,1.0);
	
}