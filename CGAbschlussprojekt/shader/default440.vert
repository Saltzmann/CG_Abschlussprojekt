#version 440

// Default "pass-thru" Vertex-Shader

layout(location = 0)uniform mat4 projectionMatrix;
layout(location = 4)uniform mat4 viewMatrix;
layout(location = 8)uniform mat4 modelMatrix;
layout(location = 12)uniform vec4 colorIN;

layout(location = 0)in vec4 vert;
layout(location = 1)in vec4 norm; // nicht genutzt

layout(location = 0)out vec4 color;

void main() {
    mat4 matrix = projectionMatrix * viewMatrix * modelMatrix;
    gl_Position = matrix * vert;
    color = colorIN;
}
