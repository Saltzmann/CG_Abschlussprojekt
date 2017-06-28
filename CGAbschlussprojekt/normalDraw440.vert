#version 440

layout(location = 0)uniform mat4 projectionMatrix;
layout(location = 1)uniform mat4 viewMatrix;
layout(location = 2)uniform mat4 modelMatrix;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out VS_OUT {
    vec3 normal;
    vec4 color;
}  vs_out;

void main() {
    //Vektor Ergänzung
    vec4 vert4 = vec4(position.xyz, 1);

    gl_Position = vert4;
    vs_out.normal = normal;
    vs_out.color = vec4(1.f, 1.f, 0.f, 1.f);
}
