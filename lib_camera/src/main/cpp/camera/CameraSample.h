#ifndef OPENGLESTEST_CAMERASAMPLE_H
#define OPENGLESTEST_CAMERASAMPLE_H

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "../util/GLUtils.h"
#include "../util/Log.h"

class CameraSample {
public:

    void onInit(int type);

    void onSurfaceCreated();

    void onPreviewFrame(uint8_t *buffer, int size, int w, int h);

    void onCaptureFrame(uint8_t *buffer, int size, int w, int h);

    void onPBOCreated(int w, int h);

    void onSurfaceChanged(int w, int h);

    void onDraw();

    void onDestroy();

    static CameraSample *instance();

protected:
    int m_Type = 1;
    GLuint m_Program_Camera;
    GLuint m_Texture_Camera[3];
    GLuint m_VBO_Camera[3];
    GLuint m_VAO_Camera[1];
    GLuint m_PBO_In[6];
    GLuint m_PBO_Out[6];
    int m_PBO = -1;
    uint8_t *m_buffer;
    int m_w;
    int m_h;
private:
    static CameraSample *m_Sample;
};


#endif //OPENGLESTEST_CAMERASAMPLE_H
