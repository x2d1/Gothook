//
// Created by Lenovo on 2025/2/26.
//
#include<jni.h>

#ifndef GOT_HOOK_HELLO_H
#define GOT_HOOK_HELLO_H
#ifdef __cplusplus
extern "C" {
#endif
JNIEXPORT void JNICALL Java_com_example_got_1hook_MainActivity_hello(JNIEnv *env, jobject thiz);
#ifdef __cplusplus
}
#endif
#endif