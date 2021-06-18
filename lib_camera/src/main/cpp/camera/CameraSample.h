#ifndef OPENGLESTEST_CAMERASAMPLE_H
#define OPENGLESTEST_CAMERASAMPLE_H

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "../util/GLUtils.h"
#include "../util/Log.h"

class CameraSample {
public:
    void onSurfaceCreated();

    void onPreviewFrame(uint8_t *buffer, int w, int h);

    void onCaptureFrame(uint8_t *buffer, int w, int h);

    void onSurfaceChanged(int w, int h);

    void onDraw();

    void onDestroy();

    static CameraSample *instance();

protected:
    GLuint m_Program_Camera;
    GLuint m_Texture_Camera[3];
    GLuint m_VBO_Camera[3];
    GLuint m_VAO_Camera[1];
    uint8_t *m_buffer;
    int m_w;
    int m_h;
private:
    static CameraSample *m_Sample;
};


#endif //OPENGLESTEST_CAMERASAMPLE_H
