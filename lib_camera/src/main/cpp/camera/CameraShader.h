#ifndef OPENGLESTEST_CAMERASHADER_H
#define OPENGLESTEST_CAMERASHADER_H

#define GL_SHADER_VERSION "#version 300 es \n"
#define GL_SHADER(x) #x
const char *ShaderVertex =
        GL_SHADER_VERSION
        GL_SHADER(
                layout(location = 0) in vec4 viPosition;
                layout(location = 1) in vec2 viTexCoord;
                out vec2 fiTexCoord;
                uniform mat4 vMatrix;
                void main() {
                    gl_Position = viPosition * vMatrix;
                    fiTexCoord = viTexCoord;
                }
        );
const char *ShaderFragment =
        GL_SHADER_VERSION
        GL_SHADER(
                precision highp float;
                in vec2 fiTexCoord;
                layout(location = 0) out vec4 fragColor;
                uniform sampler2D s_TextureMap;
                void main() {
                    fragColor = texture(s_TextureMap, fiTexCoord);
                }
        );
const char *ShaderVertex_FBO =
        GL_SHADER_VERSION
        GL_SHADER(
                layout(location = 0) in vec4 viPosition;
                layout(location = 1) in vec2 viTexCoord;
                out vec2 fiTexCoord;
                uniform mat4 vMatrix;
                void main() {
                    gl_Position = viPosition;
                    fiTexCoord = viTexCoord;
                }
        );
const char *ShaderFragment_FBO_YUV420888 =
        GL_SHADER_VERSION
        GL_SHADER(
                precision highp float;
                in vec2 fiTexCoord;
                uniform sampler2D s_textureY;
                uniform sampler2D s_textureU;
                uniform sampler2D s_textureV;
                layout(location = 0) out vec4 fragColor;
                vec4 YUV420888toRGB(vec2 texCoord) {
                    float y = 0.0f;
                    float u = 0.0f;
                    float v = 0.0f;
                    float r = 0.0f;
                    float g = 0.0f;
                    float b = 0.0f;
                    y = texture(s_textureY, texCoord).r;
                    u = texture(s_textureU, texCoord).r;
                    v = texture(s_textureV, texCoord).r;
                    u = u - 0.5;
                    v = v - 0.5;
                    r = y + 1.403 * v;
                    g = y - 0.344 * u - 0.714 * v;
                    b = y + 1.770 * u;
                    return vec4(r, g, b, 1.0f);
                }
                void main() {
                    fragColor = YUV420888toRGB(fiTexCoord);
                }
        );
const char *ShaderFragment_FBO_YUV420888_Split =
        GL_SHADER_VERSION
        GL_SHADER(
                precision highp float;
                in vec2 fiTexCoord;
                uniform sampler2D s_textureY;
                uniform sampler2D s_textureU;
                uniform sampler2D s_textureV;
                layout(location = 0) out vec4 fragColor;
                vec4 YUV420888toRGB(vec2 texCoord) {
                    float y = 0.0f;
                    float u = 0.0f;
                    float v = 0.0f;
                    float r = 0.0f;
                    float g = 0.0f;
                    float b = 0.0f;
                    y = texture(s_textureY, texCoord).r;
                    u = texture(s_textureU, texCoord).r;
                    v = texture(s_textureV, texCoord).r;
                    u = u - 0.5;
                    v = v - 0.5;
                    r = y + 1.403 * v;
                    g = y - 0.344 * u - 0.714 * v;
                    b = y + 1.770 * u;
                    return vec4(r, g, b, 1.0f);
                }
                void main() {
                    vec2 newTexCoord = fiTexCoord;
                    if (newTexCoord.x < 0.5f) {
                        newTexCoord.x = newTexCoord.x * 2.0f;
                    } else {
                        newTexCoord.x = (newTexCoord.x - 0.5f) * 2.0f;
                    }
                    if (newTexCoord.y < 0.5f) {
                        newTexCoord.y = newTexCoord.y * 2.0f;
                    } else {
                        newTexCoord.y = (newTexCoord.y - 0.5f) * 2.0f;
                    }
                    fragColor = YUV420888toRGB(newTexCoord);
                }
        );
const char *ShaderFragment_FBO_YUV420888_ColorOffset =
        GL_SHADER_VERSION
        GL_SHADER(
                precision highp float;
                in vec2 fiTexCoord;
                uniform sampler2D s_textureY;
                uniform sampler2D s_textureU;
                uniform sampler2D s_textureV;
                layout(location = 0) out vec4 fragColor;
                uniform float fOffset;
                vec4 YUV420888toRGB(vec2 texCoord) {
                    float y = 0.0f;
                    float u = 0.0f;
                    float v = 0.0f;
                    float r = 0.0f;
                    float g = 0.0f;
                    float b = 0.0f;
                    y = texture(s_textureY, texCoord).r;
                    u = texture(s_textureU, texCoord).r;
                    v = texture(s_textureV, texCoord).r;
                    u = u - 0.5;
                    v = v - 0.5;
                    r = y + 1.403 * v;
                    g = y - 0.344 * u - 0.714 * v;
                    b = y + 1.770 * u;
                    return vec4(r, g, b, 1.0f);
                }
                void main() {
                    vec4 color = YUV420888toRGB(fiTexCoord);
                    vec4 color1 = YUV420888toRGB(
                            vec2(fiTexCoord.x + fOffset, fiTexCoord.y + fOffset));
                    vec4 color2 = YUV420888toRGB(
                            vec2(fiTexCoord.x - fOffset, fiTexCoord.y - fOffset));
                    fragColor = vec4(color.r, color1.g, color2.b, 1.0f);
                }
        );
const char *ShaderFragment_FBO_YUV420888_SoulOut =
        GL_SHADER_VERSION
        GL_SHADER(
                precision highp float;
                in vec2 fiTexCoord;
                uniform sampler2D s_textureY;
                uniform sampler2D s_textureU;
                uniform sampler2D s_textureV;
                layout(location = 0) out vec4 fragColor;
                uniform float fOffset;
                vec4 YUV420888toRGB(vec2 texCoord) {
                    float y = 0.0f;
                    float u = 0.0f;
                    float v = 0.0f;
                    float r = 0.0f;
                    float g = 0.0f;
                    float b = 0.0f;
                    y = texture(s_textureY, texCoord).r;
                    u = texture(s_textureU, texCoord).r;
                    v = texture(s_textureV, texCoord).r;
                    u = u - 0.5;
                    v = v - 0.5;
                    r = y + 1.403 * v;
                    g = y - 0.344 * u - 0.714 * v;
                    b = y + 1.770 * u;
                    return vec4(r, g, b, 1.0f);
                }
                const float MAX_ALPHA = 0.5;
                const float MAX_SCALE = 0.8;
                void main() {
                    float alpha = MAX_ALPHA * (1.0 - fOffset);
                    float scale = 1.0 + fOffset * MAX_SCALE;
                    float scaleX = 0.5 + (fiTexCoord.x - 0.5) / scale;
                    float scaleY = 0.5 + (fiTexCoord.y - 0.5) / scale;
                    vec2 scaleCoord = vec2(scaleX, scaleY);
                    vec4 scaleColor = YUV420888toRGB(scaleCoord);
                    vec4 originColor = YUV420888toRGB(fiTexCoord);
                    fragColor = originColor * (1.0 - alpha) + scaleColor * alpha;
                }
        );
const char *ShaderFragment_FBO_YUV420888_ScaleCircle =
        GL_SHADER_VERSION
        GL_SHADER(
                precision highp float;
                in vec2 fiTexCoord;
                uniform sampler2D s_textureY;
                uniform sampler2D s_textureU;
                uniform sampler2D s_textureV;
                layout(location = 0) out vec4 fragColor;
                uniform float fOffset;
                uniform vec2 textureSize;
                vec4 YUV420888toRGB(vec2 texCoord) {
                    float y = 0.0f;
                    float u = 0.0f;
                    float v = 0.0f;
                    float r = 0.0f;
                    float g = 0.0f;
                    float b = 0.0f;
                    y = texture(s_textureY, texCoord).r;
                    u = texture(s_textureU, texCoord).r;
                    v = texture(s_textureV, texCoord).r;
                    u = u - 0.5;
                    v = v - 0.5;
                    r = y + 1.403 * v;
                    g = y - 0.344 * u - 0.714 * v;
                    b = y + 1.770 * u;
                    return vec4(r, g, b, 1.0f);
                }
                const float MAX_ALPHA = 0.5;
                const float MAX_SCALE = 0.8;
                void main() {
                    vec2 imgTexture = fiTexCoord * textureSize;
                    float r = (fOffset + 0.208) * textureSize.x;
                    if (distance(imgTexture, vec2(textureSize.x / 2.0, textureSize.y / 2.0)) < r) {
                        fragColor = YUV420888toRGB(fiTexCoord);
                    } else {
                        fragColor = vec4(1.0, 1.0, 1.0, 1.0);
                    }
                }
        );
#endif //OPENGLESTEST_CAMERASHADER_H
