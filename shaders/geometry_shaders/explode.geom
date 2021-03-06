#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 5) out;

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
} gs_in[];

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform float time;

vec3 get_normal() {
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 normal) {
    float magnitude = 2.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
    return position + vec4(direction, 0.0);
}

void main() {
    vec3 normal = get_normal();

    for (int i = 0; i < 3; ++i) {
        gl_Position = explode(gl_in[i].gl_Position, normal);
        FragPos = gs_in[i].fragPos;
        Normal = gs_in[i].normal;
        TexCoords = gs_in[i].texCoords;
        EmitVertex();
    }

    EndPrimitive();
}
