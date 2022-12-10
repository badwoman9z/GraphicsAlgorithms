#version 410 core

//in float Height;
in vec2 TexCoord;
out vec4 FragColor;
in vec3 normal;
void main()
{
	//float h = (Height + 16)/64.0f;
	FragColor = vec4(normal, 1.0);
}
