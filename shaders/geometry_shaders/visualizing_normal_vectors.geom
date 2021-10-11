#version 330 core

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in VS_OUT {
    vec4 normal;
} gs_in[];

uniform float model_scale;

const float MAGNITUDE = 0.4;

void generate_line(int index) {
    gl_Position = gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = gl_in[index].gl_Position + gs_in[index].normal * MAGNITUDE * model_scale;
    EmitVertex();
    EndPrimitive();
}

void main() {
    generate_line(0);
    generate_line(1);
    generate_line(2);
}
