#version 330 core
layout (location = 0) out vec4 RadiantFlux;	
layout (location = 1) out vec3 Normal;
layout (location = 2) out vec4 Position;

in  vec2 TexCoords;
in  vec3 WorldNormal;
in  vec3 WorldPos;

uniform sampler2D albedo;
uniform vec3 u_LightIntensity;
uniform vec3 u_LightDir;
uniform float u_Near = 0.1;
uniform float u_Far = 1000.0f;
float LinearizeDepth(float vDepth)
{
    float z = vDepth * 2.0 - 1.0; 
    return (2.0 * u_Near * u_Far) / (u_Far + u_Near - z * (u_Far - u_Near));    
}
void main(){



Normal = normalize(WorldNormal);

Position = vec4(WorldPos,LinearizeDepth(gl_FragCoord.z));

float area = 4.0/(128.0*128.0);

RadiantFlux =vec4( (u_LightIntensity * texture(albedo,TexCoords).rgb)*max(dot(Normal,normalize(-u_LightDir)),0),1.0);

//*max(dot(Normal,normalize(u_LightDir)),0)
//

}