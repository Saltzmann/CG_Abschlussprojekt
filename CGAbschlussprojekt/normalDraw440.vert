/*
#version 440

layout(location = 0)uniform mat4 projectionMatrix;
layout(location = 1)uniform mat4 viewMatrix;
layout(location = 2)uniform mat4 modelMatrix;
//layout(location = 3)uniform vec4 colorIN;
layout(location = 4)uniform mat3 normalMatrix;

layout (location = 0) in vec4 vert;
layout (location = 1) in vec4 norm;

out VS_OUT {
    vec3 normal;
} vs_out;

void main() {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vert;
    vs_out.normal = normalize(normalMatrix * vec3(norm));
}
*/
#version 440

layout(location = 0)uniform mat4 projectionMatrix;
layout(location = 1)uniform mat4 viewMatrix;
layout(location = 2)uniform mat4 modelMatrix;

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

out VS_OUT {
    vec3 normal;
    vec4 color;
}  vs_out;

void main() {
    gl_Position = position;
    vs_out.normal = normal;
    vs_out.color = vec4(1.f, 1.f, 0.f, 1.f);
}
