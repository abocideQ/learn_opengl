#include <jni.h>
#include "Log.h"
#include "CameraSample.h"

extern "C" {

void native_OnInit(JNIEnv *env, jobject obj, jint type) {
    CameraSample::instance()->onInit(type);
}

void native_OnSurfaceCreated(JNIEnv *env, jobject obj) {
    CameraSample::instance()->onSurfaceCreated();
}

void native_OnSurfaceChanged(JNIEnv *env, jobject obj, jint w, jint h) {
    CameraSample::instance()->onSurfaceChanged(w, h);
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

void native_OnDestroy(JNIEnv *env, jobject obj) {
    CameraSample::instance()->onDestroy();
}

const char *JNI_Classes[] = {
        "lin/abcdq/camera/CameraJni"
};
JNINativeMethod JNI_Methods[] = {
        {"native_OnInit",           "(I)V",    (void *) native_OnInit},
        {"native_OnSurfaceCreated", "()V",     (void *) native_OnSurfaceCreated},
        {"native_OnSurfaceChanged", "(II)V",   (void *) native_OnSurfaceChanged},
        {"native_OnDrawFrame",      "()V",     (void *) native_OnDrawFrame},
        {"native_OnPreview",        "([BII)V", (void *) native_OnPreview},
        {"native_OnDestroy",        "()V",     (void *) native_OnDestroy},
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