#version 330 core
uniform sampler2D albedo;
layout (location = 0) out vec4 gAlbedoSpec;


in vec2 TexCoord;
//out vec4 fragColor;

void main(void)
{
	gAlbedoSpec = vec4(texture(albedo,TexCoord).rgb,1.0);
	
}
