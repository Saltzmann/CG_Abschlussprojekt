#version 440

uniform sampler2D refractionBackground;
uniform sampler2D refractionOverlay;
uniform sampler2D dropAlpha;
uniform sampler2D dropColor;

layout(location = 0)in vec4 texC;
layout(location = 0)out vec4 fragColor;

// getAlpha(vec2 coordinate) {
//
//}

void main() {
    //vec4 tex1 = texture(diffuseMap, texC.xy);
    //vec4 tex2 = texture(blendMap, texC.xy);
    //fragColor = mix(tex1, tex2, 0.5);

    fragColor = texture(dropColor, texC.xy);
}
