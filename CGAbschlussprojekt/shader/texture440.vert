#version 440

layout(location = 0)uniform mat4 projectionMatrix;
layout(location = 1)uniform mat4 viewMatrix;
layout(location = 2)uniform mat4 modelMatrix;
layout(location = 0)in vec4 vert;
layout(location = 1)in vec4 norm;
layout(location = 2)in vec4 texCoord;
layout(location = 0)out vec4 texC;

void main() {
    mat4 matrix = projectionMatrix * viewMatrix * modelMatrix;
    gl_Position = matrix * vert;
    texC = texCoord;
}
