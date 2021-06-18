#include <jni.h>
#include "Log.h"
#include "CameraSample.h"

extern "C" {

void native_OnInit(JNIEnv *env, jobject obj) {
    CameraSample::instance()->onSurfaceCreated();
}

void native_OnDrawFrame(JNIEnv *env, jobject obj) {
    CameraSample::instance()->onDraw();
}
void native_OnPreview(JNIEnv *env, jobject obj, jbyteArray dataBytes, jint w, jint h) {
    int length = env->GetArrayLength(dataBytes);
    uint8_t *buffer = new uint8_t[length];
    env->GetByteArrayRegion(dataBytes, 0, length, reinterpret_cast<jbyte *>(buffer));
    CameraSample::instance()->onPreviewFrame(buffer, w, h);
//    env->DeleteLocalRef(dataBytes);
}

const char *JNI_Classes[] = {
        "lin/abcdq/camera/CameraJni"
};
JNINativeMethod JNI_Methods[] = {
        {"native_OnInit",      "()V",     (void *) native_OnInit},
        {"native_OnDrawFrame", "()V",     (void *) native_OnDrawFrame},
        {"native_OnPreview",   "([BII)V", (void *) native_OnPreview},
};
#define JNI_LENGTH(n) (sizeof(n)/sizeof(n[0]))
jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    jclass clazz = env->FindClass(JNI_Classes[0]);
    if (env->RegisterNatives(clazz, JNI_Methods, JNI_LENGTH(JNI_Methods)) != JNI_OK) {
        return JNI_ERR;
    }
    return JNI_VERSION_1_6;
}
}