#version 430 core
out vec4 FragColor;

uniform samplerCube skybox;
uniform sampler2D brdf;
uniform vec3 viewPos;
uniform float roughness;
uniform float metalness;
in vec3 wordPos;
in vec3 normal;


void main()
{    
    vec3 wo = normalize(viewPos-wordPos);
    vec3 wi = normalize(reflect(-wo,normal));

    vec3 albedo = vec3(0.91, 0.92, 0.92);
    vec3 m = vec3(metalness);
    vec3 F0 = mix(vec3(0.04), albedo, metalness);


    vec2 prebrdf = texture(brdf,vec2(max(dot(normal, wo), 0.0),roughness)).rg;

    vec3 light =  textureLod(skybox,wi,roughness*4.0).rgb;
    vec3 color = light*(F0*prebrdf.x+prebrdf.y);
    FragColor = vec4(color,1.0);
}