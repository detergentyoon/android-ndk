//
// Created by eugenecha on 2024-01-03.
//

#include <jni.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <android/log.h>

#define LOG_TAG "Dynamic_Library"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

JNIEXPORT jstring JNICALL
Java_com_example_ndk_1sample_android_1process_DynamicLinkerActivity_stringFromJNI(
        JNIEnv *env, jobject obj) {
    char buffer[256] = {0}; // 버퍼 초기화
    double (*cosine)(double); // 함수 변수
    char *error;

    // 운영 체제는 설치된 경로를 통해 라이브러리를 찾음
    // libm.so 라이브러리가 존재하는 경로는 "/system/lib"
    void *handle = dlopen ("libm.so", RTLD_NOW);
    if (!handle) {
        sprintf(buffer, "dlopen() 함수: %s\n", dlerror());
    }

    // 두 번째 인수로 사용하고자 하는 함수를 찾아 메모리 주소를 반환받음
    cosine = dlsym(handle, "cos");
    // NULL 반환 시 에러 발생을 의미
    if (cosine == NULL) {
        sprintf(buffer, "dlsym() 함수 : %s\n", dlerror());
        return (*env)->NewStringUTF(env, buffer);
    }

    // 함수 호출
    sprintf(buffer, "값 %f cos(2.0)\n", (*cosine)(2.0));

    // 메모리에 로드한 라이브러리 닫기
    dlclose(handle);

    return (*env)->NewStringUTF(env, buffer);
}