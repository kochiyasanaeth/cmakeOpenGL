#version 430

in vec4 varyingColor;
out vec4 fragColor;
struct PositionalLight {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 position;
};
struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};
uniform vec4 globalAmbient;
uniform PositionalLight light;
uniform Material material;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_martix;
in vec3 varyingNormal;
in vec3 varyingLightDir;
in vec3 varyingVertPos;
in vec3 varyingHalfV;
in vec2 tc;
layout (binding = 0) uniform sampler2D samp;

void main(void) {
    vec3 L = normalize(varyingLightDir);
    vec3 N = normalize(varyingNormal);
    vec3 V = normalize(-varyingVertPos);
    vec3 H = normalize(varyingHalfV);
    float cosTheta = dot(L,N);
    vec4 texcolor = texture(samp,tc);
    fragColor = texcolor * (globalAmbient + light.ambient + light.diffuse * max(dot(L,N),0.0)) + light.specular * pow(max(dot(H,N),0.0),material.shininess * 3.0);
}