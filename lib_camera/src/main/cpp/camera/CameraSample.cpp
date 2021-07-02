#include "CameraSample.h"
#include "CameraShader.h"

extern "C" {
//#define IN_PIXELS_BY_PBO true
//#define OUT_PIXELS_BY_PBO true
const float LOCATION_VERTEX_CAMERA[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
};
const float LOCATION_TEXTURE_CAMERA[] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
};
const float LOCATION_TEXTURE_CAMERA_FBO[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
};
const int LOCATION_INDICES[] = {
        0, 1, 2, 1, 3, 2
};

void CameraSample::onInit(int type) {
    m_Type = type;
}

void CameraSample::onSurfaceCreated() {
    m_Program_Camera = GLUtils::glProgram(ShaderVertex, ShaderFragment);
    if (m_Type == 1) {
        m_Program_Camera_FBO = GLUtils::glProgram(ShaderVertex_FBO,
                                                  ShaderFragment_FBO_YUV420888);
    } else if (m_Type == 2) {
        m_Program_Camera_FBO = GLUtils::glProgram(ShaderVertex_FBO,
                                                  ShaderFragment_FBO_YUV420888_Split);
    } else if (m_Type == 3) {
        m_Program_Camera_FBO = GLUtils::glProgram(ShaderVertex_FBO,
                                                  ShaderFragment_FBO_YUV420888_ColorOffset);
    } else if (m_Type == 4) {
        m_Program_Camera_FBO = GLUtils::glProgram(ShaderVertex_FBO,
                                                  ShaderFragment_FBO_YUV420888_SoulOut);
    } else if (m_Type == 5) {
        m_Program_Camera_FBO = GLUtils::glProgram(ShaderVertex_FBO,
                                                  ShaderFragment_FBO_YUV420888_ScaleCircle);
    }
    if (m_Program_Camera == GL_NONE) return;
    if (m_Program_Camera_FBO == GL_NONE) return;

    glGenBuffers(4, m_VBO_Camera);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Camera[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(LOCATION_VERTEX_CAMERA), LOCATION_VERTEX_CAMERA,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Camera[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(LOCATION_TEXTURE_CAMERA), LOCATION_TEXTURE_CAMERA,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Camera[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(LOCATION_TEXTURE_CAMERA_FBO), LOCATION_TEXTURE_CAMERA_FBO,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO_Camera[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(LOCATION_INDICES), LOCATION_INDICES,
                 GL_STATIC_DRAW);
    GL_ERROR_CHECK();

    //offscreen vao+fbo+texture
    glGenVertexArrays(1, m_VAO_Camera_FBO);
    glBindVertexArray(m_VAO_Camera_FBO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Camera[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void *) 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Camera[2]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (void *) 0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO_Camera[3]);

    glGenTextures(1, m_Texture_Camera_FBO);
    glBindTexture(GL_TEXTURE_2D, m_Texture_Camera_FBO[0]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GL_ERROR_CHECK();

    glGenFramebuffers(1, m_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO[0]);
    glBindTexture(GL_TEXTURE_2D, m_Texture_Camera_FBO[0]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           m_Texture_Camera_FBO[0], 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOGCATE("gl_error::CreateFrameBufferObj status != GL_FRAMEBUFFER_COMPLETE");
    }
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);

    //normal vao+teture
    glGenVertexArrays(1, m_VAO_Camera);
    glBindVertexArray(m_VAO_Camera[0]);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Camera[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void *) 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Camera[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (void *) 0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO_Camera[3]);

    glGenTextures(3, m_Texture_Camera);
    glBindTexture(GL_TEXTURE_2D, m_Texture_Camera[0]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, m_Texture_Camera[1]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, m_Texture_Camera[2]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GL_ERROR_CHECK();
    //pbo
#ifdef IN_PIXELS_BY_PBO
    glGenBuffers(6, m_PBO_In);
    glGenBuffers(2, m_PBO_Out);
    GL_ERROR_CHECK();
#endif
}

void CameraSample::onPreviewFrame(uint8_t *buffer, int size, int w, int h) {
    m_buffer = buffer;
    m_w = w;
    m_h = h;
}

uint8_t *CameraSample::onCaptureFrame() {
    return m_FboBuffer;
}

int CameraSample::onCaptureFrameLength() {
    return m_FboBufferSize;
}

void CameraSample::onSurfaceChanged(int w, int h) {
    m_diplay_w = w;
    m_diplay_h = h;
}

void CameraSample::onDraw() {
    if (m_Program_Camera == GL_NONE) return;
    if (m_Program_Camera_FBO == GL_NONE) return;
    if (m_buffer == nullptr) return;
    if (m_w == 0) return;
    if (m_h == 0) return;
    if (m_diplay_w == 0) return;
    if (m_diplay_h == 0) return;
#ifdef IN_PIXELS_BY_PBO
    //texture data pbo
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_PBO_In[0]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, m_w * m_h, 0, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_PBO_In[1]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, m_w * m_h, 0, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_PBO_In[2]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, m_w * m_h / 4, 0, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_PBO_In[3]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, m_w * m_h / 4, 0, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_PBO_In[4]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, m_w * m_h / 4, 0, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_PBO_In[5]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, m_w * m_h / 4, 0, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, m_PBO_Out[0]);
    glBufferData(GL_PIXEL_PACK_BUFFER, m_w * m_h * 4, 0, GL_STREAM_READ);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, m_PBO_Out[1]);
    glBufferData(GL_PIXEL_PACK_BUFFER, m_w * m_h * 4, 0, GL_STREAM_READ);
//        glBindBuffer(GL_PIXEL_PACK_BUFFER, m_PBO_Out[2]);
//        glBufferData(GL_PIXEL_PACK_BUFFER, m_w * m_h / 4, 0, GL_STREAM_READ);
//        glBindBuffer(GL_PIXEL_PACK_BUFFER, m_PBO_Out[3]);
//        glBufferData(GL_PIXEL_PACK_BUFFER, m_w * m_h / 4, 0, GL_STREAM_READ);
//        glBindBuffer(GL_PIXEL_PACK_BUFFER, m_PBO_Out[4]);
//        glBufferData(GL_PIXEL_PACK_BUFFER, m_w * m_h / 4, 0, GL_STREAM_READ);
//        glBindBuffer(GL_PIXEL_PACK_BUFFER, m_PBO_Out[5]);
//        glBufferData(GL_PIXEL_PACK_BUFFER, m_w * m_h / 4, 0, GL_STREAM_READ);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    int oddIndex = m_PBO_Index % 2;
    m_PBO_Index++;
    int index = oddIndex;
    int nextIndex = (index + 1) % 2;
    int size = m_w * m_h;
    glBindTexture(GL_TEXTURE_2D, m_Texture_Camera[0]);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_PBO_In[index]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_w, m_h, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, 0);
//        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_w, m_h, GL_LUMINANCE, GL_UNSIGNED_BYTE, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_PBO_In[nextIndex]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, size, nullptr, GL_STREAM_DRAW);
    GLubyte *ptr1 = (GLubyte *) glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0,
                                                 size,
                                                 GL_MAP_WRITE_BIT |
                                                 GL_MAP_INVALIDATE_BUFFER_BIT);
    if (ptr1) {
        memcpy(ptr1, m_buffer, static_cast<size_t>( size));
        glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
    }
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    size = m_w * m_h / 4;
    index = oddIndex + 2;
    nextIndex = (index + 1) % 2 + 2;
    glBindTexture(GL_TEXTURE_2D, m_Texture_Camera[1]);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_PBO_In[index]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_w / 2, m_h / 2, 0, GL_LUMINANCE,
                 GL_UNSIGNED_BYTE, 0);
//        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_w / 2, m_h / 2, GL_LUMINANCE, GL_UNSIGNED_BYTE, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_PBO_In[nextIndex]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, size, nullptr, GL_STREAM_DRAW);
    GLubyte *ptr2 = (GLubyte *) glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0,
                                                 size,
                                                 GL_MAP_WRITE_BIT |
                                                 GL_MAP_INVALIDATE_BUFFER_BIT);
    if (ptr2) {
        memcpy(ptr2, m_buffer + (m_w * m_h), static_cast<size_t>( size));
        glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
    }
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    size = m_w * m_h / 4;
    index = oddIndex + 4;
    nextIndex = (index + 1) % 2 + 4;
    glBindTexture(GL_TEXTURE_2D, m_Texture_Camera[2]);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_PBO_In[index]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_w / 2, m_h / 2, 0, GL_LUMINANCE,
                 GL_UNSIGNED_BYTE, 0);
//        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_w / 2, m_h / 2, GL_LUMINANCE, GL_UNSIGNED_BYTE, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_PBO_In[nextIndex]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, size, nullptr, GL_STREAM_DRAW);
    GLubyte *ptr3 = (GLubyte *) glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0,
                                                 size,
                                                 GL_MAP_WRITE_BIT |
                                                 GL_MAP_INVALIDATE_BUFFER_BIT);
    if (ptr3) {
        memcpy(ptr3, m_buffer + (m_w * m_h) + (m_w * m_h / 4), static_cast<size_t>( size));
        glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
    }
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    if (m_PBO_Index < 10)return; //防绿屏
#else
    //texture data without pbo
    glBindTexture(GL_TEXTURE_2D, m_Texture_Camera[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_w, m_h, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE,
                 m_buffer);
    glBindTexture(GL_TEXTURE_2D, m_Texture_Camera[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_w / 2, m_h / 2, 0, GL_LUMINANCE,
                 GL_UNSIGNED_BYTE,
                 m_buffer + (m_w * m_h));
    glBindTexture(GL_TEXTURE_2D, m_Texture_Camera[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_w / 2, m_h / 2, 0, GL_LUMINANCE,
                 GL_UNSIGNED_BYTE,
                 m_buffer + (m_w * m_h) + (m_w * m_h / 4));
#endif
    //offscreen
    glBindTexture(GL_TEXTURE_2D, m_Texture_Camera_FBO[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_w, m_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glViewport(0, 0, m_w, m_h);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO[0]);
    glUseProgram(m_Program_Camera_FBO);
    glBindVertexArray(m_VAO_Camera_FBO[0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_Texture_Camera[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_Texture_Camera[1]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_Texture_Camera[2]);
    GLint textureY = glGetUniformLocation(m_Program_Camera_FBO, "s_textureY");
    GLint textureU = glGetUniformLocation(m_Program_Camera_FBO, "s_textureU");
    GLint textureV = glGetUniformLocation(m_Program_Camera_FBO, "s_textureV");
    glUniform1i(textureY, 0);
    glUniform1i(textureU, 1);
    glUniform1i(textureV, 2);
    onChangeOffset();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void *) 0);
#ifdef OUT_PIXELS_BY_PBO
    size = m_w * m_h * 4;
    index = oddIndex;
    nextIndex = (index + 1) % 2;
    glBindBuffer(GL_PIXEL_PACK_BUFFER, m_PBO_Out[index]);
    glReadPixels(0, 0, m_w, m_h, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, m_PBO_Out[nextIndex]);
    GLubyte *ptrOut = (GLubyte *) glMapBufferRange(GL_PIXEL_PACK_BUFFER, 0,
                                                   size,
                                                   GL_MAP_READ_BIT);
    if (ptrOut) {
        m_FboBuffer = ptrOut;
        m_FboBufferSize = size;
        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
    }
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
#endif
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //normal
    glViewport(0, 0, m_diplay_w, m_diplay_h);
    glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glUseProgram(m_Program_Camera);
    glBindVertexArray(m_VAO_Camera[0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_Texture_Camera_FBO[0]);
    GLint textureMap = glGetUniformLocation(m_Program_Camera, "s_TextureMap");
    glUniform1i(textureMap, 0);

    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                       glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::rotate(view, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 mat4Matrix = glm::mat4(1.0f);
    mat4Matrix = projection * view * model;
    unsigned int vMatrix = glGetUniformLocation(m_Program_Camera, "vMatrix");
    glUniformMatrix4fv(vMatrix, 1, GL_FALSE, glm::value_ptr(mat4Matrix));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void *) 0);
}

void CameraSample::onChangeOffset() {
    //after glUseProgram()
    float randomOffset = -1.0f * (((float) rand() / float(RAND_MAX + 1)) / 100.0f);
    if (m_Type == 3) {
        GLint offsetHandler = glGetUniformLocation(m_Program_Camera_FBO, "fOffset");
        m_Offset += randomOffset;
        if (m_Offset >= 1) m_Offset = 0;
        glUniform1f(offsetHandler, m_Offset);
    } else if (m_Type == 4) {
        GLint offsetHandler = glGetUniformLocation(m_Program_Camera_FBO, "fOffset");
        m_Offset += randomOffset;
        if (m_Offset >= 1) m_Offset = 0;
        glUniform1f(offsetHandler, m_Offset);
    } else if (m_Type == 5) {
        GLint texSizeHandler = glGetUniformLocation(m_Program_Camera_FBO, "textureSize");
        GLfloat size[2];
        size[0] = m_w;
        size[1] = m_h;
        glUniform2fv(texSizeHandler, 1, &size[0]);
        GLint offsetHandler = glGetUniformLocation(m_Program_Camera_FBO, "fOffset");
        m_Offset += randomOffset;
        if (m_Offset >= 1) m_Offset = 0;
        glUniform1f(offsetHandler, m_Offset);
    }
}

void CameraSample::onDestroy() {
    if (m_VBO_Camera[0]) {
        glDeleteBuffers(3, m_VBO_Camera);
    }
    if (m_Texture_Camera_FBO[0]) {
        glDeleteTextures(1, m_Texture_Camera_FBO);
    }
    if (m_VAO_Camera_FBO[0]) {
        glDeleteVertexArrays(1, m_VAO_Camera_FBO);
    }
    if (m_FBO[0]) {
        glDeleteFramebuffers(1, m_FBO);
    }
    if (m_Program_Camera_FBO) {
        glDeleteProgram(m_Program_Camera_FBO);
        m_Program_Camera_FBO = GL_NONE;
    }
    if (m_PBO_In[0]) {
        glDeleteBuffers(6, m_PBO_In);
    }
    if (m_PBO_Out[0]) {
        glDeleteBuffers(6, m_PBO_In);
    }
    if (m_Texture_Camera[0]) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(3, m_Texture_Camera);
    }
    if (m_VAO_Camera[0]) {
        glDeleteVertexArrays(1, m_VAO_Camera);
    }
    if (m_Program_Camera) {
        glDeleteProgram(m_Program_Camera);
//        m_Program_Camera = GL_NONE;
    }
    m_Sample = nullptr;
}

CameraSample *CameraSample::m_Sample = nullptr;
CameraSample *CameraSample::instance() {
    if (m_Sample == nullptr) m_Sample = new CameraSample();
    return m_Sample;
}
}







