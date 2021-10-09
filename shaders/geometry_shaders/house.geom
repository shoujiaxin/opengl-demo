#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 5) out;

in VS_OUT {
    vec3 color;
} gs_in[];

out vec3 ourColor;

void build_house(vec4 position) {
    ourColor = gs_in[0].color;
    // 左下
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);
    EmitVertex();
    // 右下
    gl_Position = position + vec4(0.2, -0.2, 0.0, 0.0);
    EmitVertex();
    // 左上
    gl_Position = position + vec4(-0.2, 0.2, 0.0, 0.0);
    EmitVertex();
    // 右上
    gl_Position = position + vec4(0.2, 0.2, 0.0, 0.0);
    EmitVertex();
    // 顶部
    gl_Position = position + vec4(0.0, 0.4, 0.0, 0.0);
    ourColor = vec3(1.0);
    EmitVertex();
    EndPrimitive();
}

void main() {
    build_house(gl_in[0].gl_Position);
}
