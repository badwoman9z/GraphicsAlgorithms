#version 460 core
#define INV_STEP_LENGTH (1.0f/STEP_LENGTH)
#define STEP_LENGTH 0.005f

uniform sampler2D textureBack; // Unit cube back FBO.
uniform sampler2D textureFront; // Unit cube front FBO.
uniform sampler3D textureColor; // Texture in which voxelization is stored.
uniform vec3 cameraPosition; // World camera position.
uniform float u_VoxelScale;
in  vec2 v2f_TexCoords;

out vec4 color;

// Scales and bias a given vector (i.e. from [-1, 1] to [0, 1]).
vec3 scaleAndBias(vec3 p) { return 0.5f * p + vec3(0.5f); }

// Returns true if p is inside the unity cube (+ e) centered on (0, 0, 0).
bool isInsideCube(vec3 p, float e) { return abs(p.x) < 1 + e && abs(p.y) < 1 + e && abs(p.z) < 1 + e; }
void main(){

	const float mipmapLevel = 0;



	// Initialize ray.
	const vec3 origin = isInsideCube(cameraPosition, 0.2f) ? 
		cameraPosition : texture(textureFront, v2f_TexCoords).xyz;
	vec3 direction = texture(textureBack, v2f_TexCoords).xyz - origin;
	const uint numberOfSteps = uint(INV_STEP_LENGTH * length(direction));
	direction = normalize(direction);

	// Trace.
	color = vec4(0.0f);
	for(uint step = 0; step < numberOfSteps && color.a < 0.99f; ++step) {
		const vec3 currentPoint = origin + STEP_LENGTH * step * direction;
		vec3 coordinate = scaleAndBias(currentPoint);
		vec4 currentSample = textureLod(textureColor, scaleAndBias(currentPoint), mipmapLevel);
		color += (1.0f - color.a) * currentSample;
	} 
	color.rgb = pow(color.rgb, vec3(1.0 / 2.2));
	


}