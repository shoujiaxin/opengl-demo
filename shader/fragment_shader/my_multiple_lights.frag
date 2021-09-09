#version 330 core

#define MAX_LIGHTS 4

struct Material {
    sampler2D diffuse;// 漫反射贴图
    sampler2D specular;// 镜面光贴图
    float shininess;// 镜面高光半径
};

struct AmbientLight {
    vec3 color;
    float intensity;
};

struct DirectionalLight {
    vec3 color;
    float intensity;

    vec3 direction;
};

struct PointLight {
    vec3 color;
    float intensity;

    vec3 position;

    float constant;// 点光源衰减常数
    float linear;// 点光源衰减一次项系数
    float quadratic;// 点光源衰减二次项系数
};

struct Spotlight {
    vec3 color;
    float intensity;

    vec3 position;
    vec3 direction;

    float innerCutOff;// 内切光角
    float outerCutOff;// 外切光角

    float constant;// 点光源衰减常数
    float linear;// 点光源衰减一次项系数
    float quadratic;// 点光源衰减二次项系数
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform Material material;
uniform AmbientLight ambientLight;
uniform DirectionalLight directionalLights[MAX_LIGHTS];
uniform PointLight pointLights[MAX_LIGHTS];
uniform Spotlight spotlight;

uniform vec3 viewPos;

vec3 CalculateAmbientLight(AmbientLight light) {
    return light.color * light.intensity * vec3(texture(material.diffuse, TexCoords));
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 合并结果
    vec3 diffuse = light.color * light.intensity * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.color * light.intensity * spec * vec3(texture(material.specular, TexCoords));
    return diffuse + specular;
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 衰减
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    // 合并结果
    vec3 diffuse = light.color * light.intensity * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.color * light.intensity * spec * vec3(texture(material.specular, TexCoords));
    diffuse *= attenuation;
    specular *= attenuation;
    return diffuse + specular;
}

vec3 CalculateSpotlight(Spotlight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // 切光角
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 衰减
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    // 合并结果
    vec3 diffuse = light.color * light.intensity * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.color * light.intensity * spec * vec3(texture(material.specular, TexCoords));
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return diffuse + specular;
}

void main() {
    // 属性
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalculateAmbientLight(ambientLight);

    for (int i = 0; i < MAX_LIGHTS; ++i) {
        result += CalculateDirectionalLight(directionalLights[i], norm, viewDir);
        result += CalculatePointLight(pointLights[i], norm, FragPos, viewDir);
    }

    result += CalculateSpotlight(spotlight, norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}
