#version 330 core

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    vec3 ambient;// 环境光照下物体的颜色（通常和物体颜色相同）
    vec3 diffuse;// 漫反射光照下物体的颜色（和环境光照一样）
    vec3 specular;// 镜面光照对物体的颜色影响
    float shininess;// 镜面高光半径
};

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;

out vec4 FragColor;

uniform Light light;
uniform Material material;

void main() {
    // 环境光照
    vec3 ambient = light.ambient * material.ambient;

    // 漫反射光照
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // 镜面光照
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
