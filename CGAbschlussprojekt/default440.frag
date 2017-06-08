#version 440

uniform sampler2D texture;
layout(location = 0)in vec4 texC;
layout(location = 0)out vec4 fragColor;

void main() {
    //gl_FragColor = texture2D(texture, texC.xy);
    gl_FragColor = vec4(1.f, 0.f, 0.f, 1.f); //Test
    //fragColor = vec4(1.f, 0.f, 0.f, 1.f); //Test
}
