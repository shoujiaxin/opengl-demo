#version 330 core

#define MAX_LIGHTS 4

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
    sampler2D texture_reflection1;
    sampler2D texture_reflection2;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform Material material;
uniform samplerCube skybox;
uniform vec3 viewPos;

void main() {
    // 属性
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 R = reflect(-viewDir, norm);

    float diff = max(dot(norm, viewDir), 0.0);
    vec3 diffuse1 = diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse2 = diff * vec3(texture(material.texture_diffuse2, TexCoords));

    vec3 reflectMap1 = vec3(texture(material.texture_reflection1, TexCoords));
    vec3 reflection1 = vec3(texture(skybox, R).rgb) * reflectMap1;
    vec3 reflectMap2 = vec3(texture(material.texture_reflection2, TexCoords));
    vec3 reflection2 = vec3(texture(skybox, R).rgb) * reflectMap2;

    FragColor = vec4(diffuse1 + diffuse2 + reflection1 + reflection2, 1.0);
}
