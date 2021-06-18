#ifndef NDKER_LOG_H
#define NDKER_LOG_H

#include<android/log.h>

#define  LOG_TAG "NDKER_LOG"
#define LOGCATE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#endif //NDKER_LOG_H
