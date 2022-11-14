#version 330 core
out vec2 FragColor;
in vec2 TexCoords;
const float PI = 3.14159265359;
float RadicalInverse_VdC(uint bits) 
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}
// ----------------------------------------------------------------------------
vec2 Hammersley(uint i, uint N)
{
    return vec2(float(i)/float(N), RadicalInverse_VdC(i));
}  
vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness){

float a = roughness*roughness;
float phi = 2.0*PI*Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta*cosTheta);

    // from spherical coordinates to cartesian coordinates
    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;

    // from tangent-space vector to world-space sample vector
    vec3 up        = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent   = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);

    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVec);



}
float G(vec3 n,vec3 v,float k){

    float nov = max(dot(n,v),0);
    return nov/((nov*(1-k))+k);

}
float Geometry(vec3 wo , vec3 N ,vec3 wi,float roughness){
float k = (roughness*roughness)/2;
return G(wo,N,k)*G(wi,N,k);

}
vec2 IntegrateBRDF(float costheta , float roughness){
    vec3 wo = vec3(0.0);
    wo.x = sqrt(1-costheta*costheta);
    wo.z = costheta;
    
    float A = 0.0;
    float B = 0.0; 
    
    vec3 N = vec3(0,0,1);
    const uint SAMPLE_COUNT = 1024u;
    for(uint i = 0;i<SAMPLE_COUNT;++i){
        vec2 Xi = Hammersley(i,SAMPLE_COUNT);
        vec3 H = ImportanceSampleGGX(Xi,N,roughness);

        vec3 wi = normalize(2.0 * dot(wo, H) * H - wo);

        float woH  = max(dot(wo,H),0.0);
        float HoN = max(dot(H,N),0.0);

        float Fc =  pow((1-woH),5);
        A+= (woH*Geometry(wo,N,wi,roughness)*(1.0-Fc))/(costheta*HoN);
        B+=(woH*Geometry(wo,N,wi,roughness)*Fc)/(costheta*HoN);
        
    }
    A /= SAMPLE_COUNT;
    B /= SAMPLE_COUNT;
    return vec2(A,B);
}

void main() 
{
    vec2 integratedBRDF = IntegrateBRDF(TexCoords.x, TexCoords.y);
    FragColor = integratedBRDF;
}