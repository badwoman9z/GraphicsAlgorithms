#version 330 core
layout (location = 0) out vec4 Position;

out vec4 FragColor;
vec4 pack (float depth) {
    // ʹ��rgba 4�ֽڹ�32λ���洢zֵ,1���ֽھ���Ϊ1/256
    const vec4 bitShift = vec4(1.0, 256.0, 256.0 * 256.0, 256.0 * 256.0 * 256.0);
    const vec4 bitMask = vec4(1.0/256.0, 1.0/256.0, 1.0/256.0, 0.0);
    // gl_FragCoord:ƬԪ������,fract():������ֵ��С������
    vec4 rgbaDepth = fract(depth * bitShift); //����ÿ�����zֵ
    rgbaDepth -= rgbaDepth.gbaa * bitMask; // Cut off the value which do not fit in 8 bits
    return rgbaDepth;
}



void main(){



 FragColor = pack(gl_FragCoord.z);


}