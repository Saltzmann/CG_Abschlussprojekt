#version 440

layout(location = 0)uniform mat4 projectionMatrix;
layout(location = 1)uniform mat4 viewMatrix;
layout(location = 2)uniform mat4 modelMatrix;
layout(location = 3)uniform vec4 colorIN;

layout(location = 0)in vec3 vert;
layout(location = 1)in vec3 norm;

layout(location = 0)out vec4 color;

void main() {
    //Vektor Ergänzung
    vec4 vert4 = vec4(vert.xyz, 1);
    vec4 norm4 = vec4(norm.xyz, 1);

    mat4 matrix = projectionMatrix * viewMatrix * modelMatrix;
    gl_Position = matrix * vert4;
    color = colorIN;
}
