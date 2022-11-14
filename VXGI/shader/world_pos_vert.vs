#version 450 core

layout(location = 0) in vec3 v_position;


uniform mat4 view;
uniform mat4 projection;
out vec3 f_world_pos;

void main()
{
	f_world_pos = v_position;
	gl_Position = projection*view * vec4(f_world_pos, 1.0f);
}