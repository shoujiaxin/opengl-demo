#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D screenTexture;

// 反相
vec4 Inversion() {
    return vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);
}

// 灰度
vec4 Grayscale() {
    vec4 color = texture(screenTexture, TexCoords);
    //    float average = (color.r + color.g + color.b) / 3.0;
    float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    return vec4(average, average, average, 1.0);
}

// 核效果
vec4 Sharpen() {
    const float offset = 1.0 / 300.0;

    vec2 offsets[9] = vec2[](
    vec2(-offset, offset), // 左上
    vec2(0.0f, offset), // 正上
    vec2(offset, offset), // 右上
    vec2(-offset, 0.0f), // 左
    vec2(0.0f, 0.0f), // 中
    vec2(offset, 0.0f), // 右
    vec2(-offset, -offset), // 左下
    vec2(0.0f, -offset), // 正下
    vec2(offset, -offset)// 右下
    );

    float kernel[9] = float[](
    -1, -1, -1,
    -1, 9, -1,
    -1, -1, -1
    );

    vec3 col = vec3(0.0);
    for (int i = 0; i < 9; ++i) {
        col += vec3(texture(screenTexture, TexCoords.st + offsets[i])) * kernel[i];
    }

    return vec4(col, 1.0);
}

// 模糊
vec4 Blur() {
    const float offset = 1.0 / 300.0;

    vec2 offsets[9] = vec2[](
    vec2(-offset, offset), // 左上
    vec2(0.0f, offset), // 正上
    vec2(offset, offset), // 右上
    vec2(-offset, 0.0f), // 左
    vec2(0.0f, 0.0f), // 中
    vec2(offset, 0.0f), // 右
    vec2(-offset, -offset), // 左下
    vec2(0.0f, -offset), // 正下
    vec2(offset, -offset)// 右下
    );

    float kernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16
    );

    vec3 col = vec3(0.0);
    for (int i = 0; i < 9; ++i) {
        col += vec3(texture(screenTexture, TexCoords.st + offsets[i])) * kernel[i];
    }

    return vec4(col, 1.0);
}

vec4 EdgeDetection() {
    const float offset = 1.0 / 300.0;

    vec2 offsets[9] = vec2[](
    vec2(-offset, offset), // 左上
    vec2(0.0f, offset), // 正上
    vec2(offset, offset), // 右上
    vec2(-offset, 0.0f), // 左
    vec2(0.0f, 0.0f), // 中
    vec2(offset, 0.0f), // 右
    vec2(-offset, -offset), // 左下
    vec2(0.0f, -offset), // 正下
    vec2(offset, -offset)// 右下
    );

    float kernel[9] = float[](
    1.0, 1.0, 1.0,
    1.0, -8.0, 1.0,
    1.0, 1.0, 1.0
    );

    vec3 col = vec3(0.0);
    for (int i = 0; i < 9; ++i) {
        col += vec3(texture(screenTexture, TexCoords.st + offsets[i])) * kernel[i];
    }

    return vec4(col, 1.0);
}

void main() {
    FragColor = EdgeDetection();
}
