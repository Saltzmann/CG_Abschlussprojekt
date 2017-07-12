#version 440

//generischer Texture Fragment Shader mit Anpassungen für das Tropfen Szenario

uniform sampler2D diffuseMap;
uniform sampler2D blendMap;

layout(location = 0)in vec4 texC;
layout(location = 0)out vec4 fragColor;

void main() {
    float darken = 0.4;
    vec4 tex1 = texture(diffuseMap, texC.xy) - vec4(darken);
    vec4 tex2 = texture(blendMap, texC.xy);
    fragColor = mix(tex1, tex2, 0.3f);
}
