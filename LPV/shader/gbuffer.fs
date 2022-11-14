#version 330 core
layout (location = 0) out vec4 Albedo;
layout (location = 1) out vec3 Normal;
layout (location = 2) out vec4 Position;
layout (location = 3) out vec4 DirectColor;
uniform sampler2D albedo;


in vec3 worldPos;
in vec3 normal;
in vec2 texCoord;
in vec4 viewPos;


uniform vec3 LightDir;
uniform vec3 LightIntensity;
uniform sampler2D rsmPosition;
uniform float u_Near = 0.1;
uniform float u_Far = 1000.0f;

float LinearizeDepth(float vDepth)
{
    float z = vDepth * 2.0 - 1.0; 
    return (2.0 * u_Near * u_Far) / (u_Far + u_Near - z * (u_Far - u_Near));    
}
float shadow( vec4 viewPos){

vec3 project = viewPos.xyz/viewPos.w;
project = project*0.5+0.5;

float closetDepth = texture(rsmPosition,project.xy).a;
float depth = LinearizeDepth(project.z);

float shadow = depth -0.00001> closetDepth ? 1.0:0.0;
return shadow;

}
void main()
{   
    Position = vec4(worldPos,LinearizeDepth(gl_FragCoord.z));

    Normal =normalize(normal);

    Albedo = texture2D(albedo,texCoord);

    vec3  L = vec3( LightIntensity *max(dot(normal,normalize(LightDir)),0.0)*texture(albedo,texCoord).rgb);
    //vec3 color = pow(clamp(L, vec3(0.0), vec3(1.0)), vec3(1.0 / 2.2));
    DirectColor = vec4( L,1.0);

    
}


























