//
// Created by Lenovo on 2025/2/26.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hello.h"
#include "logger.h"

extern "C" JNIEXPORT void JNICALL
Java_com_example_got_1hook_MainActivity_hello(JNIEnv *env, jobject thiz){
    const char* str = "hello world";
    char* buffer = (char *)malloc(100);
    if(buffer != NULL){
        strcpy(buffer,str);
    }
    LOG_d("%s\n",str);
}