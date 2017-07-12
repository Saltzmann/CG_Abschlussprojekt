#version 440

//normalDraw (Debug) Geometry-Shader - Zeichnet Linien auf den Vertex-Normalen

layout (triangles) in;
layout (line_strip) out;
layout (max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
    vec4 color;
}  gs_in[];

out GS_OUT {
    vec3 normal;
    vec4 color;
} gs_out;

layout(location = 0)uniform mat4 projectionMatrix;
layout(location = 4)uniform mat4 viewMatrix;
layout(location = 8)uniform mat4 modelMatrix;

uniform float normal_length;

//umrechnen
const mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

void drawNormals(int index) {
    gl_Position = mvp * gl_in[index].gl_Position;

    gs_out.normal = gs_in[index].normal;
    gs_out.color = gs_in[index].color;
    EmitVertex(); //Standard Vertex

    gl_Position = mvp * (gl_in[index].gl_Position +
                         vec4(gs_in[index].normal * normal_length, 0.0));
    gs_out.normal = gs_in[index].normal;
    gs_out.color = gs_in[index].color;
    EmitVertex(); //Normalen-Vertex

    EndPrimitive();
}

void main() {
    drawNormals(0);
    drawNormals(1);
    drawNormals(2);
}

/*
    vec3 ab = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 ac = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 face_normal = normalize(cross(ab, ac));

    vec4 tri_centroid = (gl_in[0].gl_Position +
                         gl_in[1].gl_Position +
                         gl_in[2].gl_Position) / 3.0;

    gl_Position = mvp * tri_centroid;
    gs_out.normal = gs_in[0].normal;
    gs_out.color = gs_in[0].color;
    EmitVertex();

    gl_Position = mvp * (tri_centroid +
                         vec4(face_normal * normal_length, 0.0));
    gs_out.normal = gs_in[0].normal;
    gs_out.color = gs_in[0].color;
    EmitVertex();
    EndPrimitive();
*/

