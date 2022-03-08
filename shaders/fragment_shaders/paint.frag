#version 330 core

out vec4 FragColor;

void main() {
    vec2 coord = gl_PointCoord - vec2(0.5f);
    if (length(coord) > 0.5) {
        discard;
    }
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
