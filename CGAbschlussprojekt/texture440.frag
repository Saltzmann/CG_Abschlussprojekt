#version 440

uniform sampler2D diffuseMap;
layout(location = 0)in vec4 texC;
layout(location = 0)out vec4 fragColor;

void main() {
    fragColor = texture2D(diffuseMap, texC.xy);
}
