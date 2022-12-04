#version 330 core
layout (location = 0) out vec4 Position;
layout (location = 1) out vec3 gNormal;
in vec3 viewPos;
in vec3 normal;

uniform float u_Near =  0.1f;
uniform float u_Far = 100.0f;
float LinearizeDepth(float vDepth)
{
    float z = vDepth * 2.0 - 1.0; 
    return (2.0 * u_Near * u_Far) / (u_Far + u_Near - z * (u_Far - u_Near));    
}
void main()
{   
    gNormal = normalize(normal);
    Position = vec4(viewPos,gl_FragCoord.z);   
}
