#version 440

uniform sampler2D diffuseMap;
layout(location = 0)in vec4 texC;
layout(location = 0)out vec4 fragColor;

void main() {
    fragColor = vec4(0.3f, 0.3f, 1.f, 1.f); //Test
}
