#version 330 core

layout(location = 0) in vec3 aPos;

out vec3 TexCoords;// 使用位置作为纹理坐标

uniform mat4 view;
uniform mat4 projection;

void main() {
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;// 使天空盒的深度 z 始终为最大值 1.0
}
