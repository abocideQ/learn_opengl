#include "CameraSample.h"

extern "C" {
const char *shaderVertexCamera =
        "#version 300 es                                \n"
        "layout(location = 0) in vec4 viPosition;       \n"
        "layout(location = 1) in vec2 viTexCoord;       \n"
        "out vec2 fiTexCoord;                           \n"
        "uniform mat4 vMatrix;                          \n"
        "void main(){                                   \n"
        "gl_Position = viPosition * vMatrix;            \n"
        "fiTexCoord = viTexCoord;                       \n"
        "}                                              \n";
const char *shaderFragmentCamera =
        "#version 300 es                                \n"
        "precision highp float;                         \n"
        "in vec2 fiTexCoord;                            \n"
        "uniform sampler2D s_textureY;                  \n"
        "uniform sampler2D s_textureU;                  \n"
        "uniform sampler2D s_textureV;                  \n"
        "out vec4 fragColor;                            \n"
        "void main(){                                   \n"
        "float y, u, v, r, g, b;                        \n"
        "y = texture(s_textureY, fiTexCoord).r;         \n"
        "u = texture(s_textureU, fiTexCoord).r;         \n"
        "v = texture(s_textureV, fiTexCoord).r;         \n"
        "u = u - 0.5;                                   \n"
        "v = v - 0.5;                                   \n"
        "r = y + 1.403 * v;                             \n"
        "g = y - 0.344 * u - 0.714 * v;                 \n"
        "b = y + 1.770 * u;                             \n"
        "fragColor = vec4(r, g, b, 1.0);                \n"
        "}                                              \n";
const float LOCATION_VERTEX_CAMERA[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
};
const float LOCATION_TEXTURE_CAMERA[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f
};
const int LOCATION_INDICES[] = {
        0, 1, 2, 1, 3, 2
};
void CameraSample::onSurfaceCreated() {
    m_Program_Camera = GLUtils::glProgram(shaderVertexCamera, shaderFragmentCamera);
    if (m_Program_Camera == GL_NONE) return;

    glGenBuffers(3, m_VBO_Camera);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Camera[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(LOCATION_VERTEX_CAMERA), LOCATION_VERTEX_CAMERA,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Camera[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(LOCATION_TEXTURE_CAMERA), LOCATION_TEXTURE_CAMERA,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO_Camera[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(LOCATION_INDICES), LOCATION_INDICES,
                 GL_STATIC_DRAW);

    glGenVertexArrays(1, m_VAO_Camera);
    glBindVertexArray(m_VAO_Camera[0]);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Camera[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void *) 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Camera[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (void *) 0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO_Camera[2]);

    glGenTextures(3, m_Texture_Camera);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_Texture_Camera[0]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 1, 1, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, NULL);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_Texture_Camera[1]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 1, 1, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, NULL);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_Texture_Camera[2]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 1, 1, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, NULL);
}

void CameraSample::onPreviewFrame(uint8_t *buffer, int w, int h) {
    m_buffer = buffer;
    m_w = w;
    m_h = h;
}

void CameraSample::onCaptureFrame(uint8_t *buffer, int w, int h) {

}

void CameraSample::onSurfaceChanged(int w, int h) {
    glViewport(0, 0, w, h);
}

void CameraSample::onDraw() {
    if (m_Program_Camera == GL_NONE) return;
    if (m_buffer == nullptr) return;
    glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0, 1.0, 1.0, 1.0);

    glUseProgram(m_Program_Camera);

    glBindVertexArray(m_VAO_Camera[0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_Texture_Camera[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_w, m_h, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE,
                 m_buffer);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_Texture_Camera[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_w / 2, m_h / 2, 0, GL_LUMINANCE,
                 GL_UNSIGNED_BYTE,
                 m_buffer + (m_w * m_h));
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_Texture_Camera[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_w / 2, m_h / 2, 0, GL_LUMINANCE,
                 GL_UNSIGNED_BYTE,
                 m_buffer + (m_w * m_h * 5 / 4));

    GLint textureY = glGetUniformLocation(m_Program_Camera, "s_textureY");
    GLint textureU = glGetUniformLocation(m_Program_Camera, "s_textureU");
    GLint textureV = glGetUniformLocation(m_Program_Camera, "s_textureV");
    glUniform1i(textureY, 0);
    glUniform1i(textureU, 1);
    glUniform1i(textureV, 2);

    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                       glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::rotate(view, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 mat4Matrix = glm::mat4(1.0f);
    mat4Matrix = projection * view * model;
    unsigned int vMatrix = glGetUniformLocation(m_Program_Camera, "vMatrix");
    glUniformMatrix4fv(vMatrix, 1, GL_FALSE, glm::value_ptr(mat4Matrix));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void *) 0);
}

void CameraSample::onDestroy() {

}

CameraSample *CameraSample::m_Sample = nullptr;
CameraSample *CameraSample::instance() {
    if (m_Sample == nullptr) m_Sample = new CameraSample();
    return m_Sample;
}
}




