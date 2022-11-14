#version 460 core
#define SH_C0 0.282094791 // 1 / 2sqrt(pi)
#define SH_C1 0.488602512 // sqrt(3/pi) / 2
 in FragData {
  vec3 cellCoord;
  vec3 N;
}
fs_in;

uniform sampler3D LPVGridR;
uniform sampler3D LPVGridG;
uniform sampler3D LPVGridB;

uniform vec3 u_Dimensions;
uniform float u_CellSize;

vec4 SH_evaluate(vec3 direction) {
  direction = normalize(direction);
  return vec4(SH_C0, -SH_C1 * direction.y, SH_C1 * direction.z,
              -SH_C1 * direction.x);
}

out vec4 FragColor;
struct SHcoeffs {
  vec4 red, green, blue;
};
void main() {
  const vec4 SH_intensity = SH_evaluate(normalize(fs_in.N));

  // clang-format off
  const SHcoeffs coeffs = {
    texture(LPVGridR, fs_in.cellCoord, 0),
    texture(LPVGridG, fs_in.cellCoord, 0),
    texture(LPVGridB, fs_in.cellCoord, 0)
  };
  // clang-format on
  const vec3 intensity =
    vec3(dot(SH_intensity, coeffs.red), dot(SH_intensity, coeffs.green),
         dot(SH_intensity, coeffs.blue));

  FragColor.rgb = intensity;
}

				 