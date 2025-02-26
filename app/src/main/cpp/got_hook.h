//
// Created by Lenovo on 2025/2/26.
//
#include<jni.h>

#ifndef GOT_HOOK_GOT_HOOK_H
#define GOT_HOOK_GOT_HOOK_H
#ifdef __cplusplus

extern "C"{
#endif
JNIEXPORT void JNICALL
Java_com_example_got_1hook_MainActivity_hook(JNIEnv *env, jobject thiz,jstring file);
#ifdef __cplusplus
}
#endif
#endif

