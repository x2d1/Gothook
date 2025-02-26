
#include<android/log.h>

#define LOG_TAG "native hook:"
#define LOG_d(format,...) __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, format, ##__VA_ARGS__)
