#version 430
out vec4 color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
/*noperspective */in vec2 tc;
layout (binding = 0) uniform sampler2D samp;

void main(void) {
    color = texture(samp,tc);
}