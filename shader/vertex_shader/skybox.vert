#version 330 core

layout(location = 0) in vec3 aPos;

out vec3 TexCoords;// 使用位置作为纹理坐标

uniform mat4 view;
uniform mat4 projection;

void main() {
    TexCoords = aPos;
    gl_Position = projection * view * vec4(aPos, 1.0);
}
