#version 330 core

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
};

in vec2 TexCoords;

out vec4 FragColor;

uniform Material material;

void main() {
    FragColor = texture(material.texture_diffuse1, TexCoords);
}