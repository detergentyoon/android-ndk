//
// Created by EugeneCha on 2024-01-04.
//

#include <jni.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <android/log.h>

#define LOG_TAG "Android_Memory"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static char *buffer;

JNIEXPORT jstring JNICALL
Java_com_example_ndk_1sample_android_1process_MemoryOptimizationActivity_stringFromJNI(
        JNIEnv *env, jobject obj) {

    // 스택 크기를 64MB로 설정
    const rlim_t kStackSize = 64L * 1024L * 1024L;
    struct rlimit rl;
    int result;
    char strp[100];

    // 스택으로 설정된 값 읽기
    result = getrlimit(RLIMIT_STACK, &rl);
    sprintf(buffer, "수정 전 limit : %lu / %lu\n", rl.rlim_cur, rl.rlim_max);

    if (result == 0 && rl.rlim_cur < kStackSize) {
        rl.rlim_cur = kStackSize; // 기본 값 수정

        // 스택 크기 재설정
        result = setrlimit(RLIMIT_STACK, &rl);
        if (result == 0) {
            result = getrlimit(RLIMIT_STACK, &rl);
            sprintf(strp, "수정 후 limit : %lu / %lu\n", rl.rlim_cur, rl.rlim_max);
            strcat(buffer, strp);
        } else {
            sprintf(strp, "setrlimit returned result = %d\n", result);
            strcat(buffer, strp);
        }
    }

    /**
     * int getrlimit(int resource, struct rlimit *rlim) : <sys/resource.h>
     * int setrlimit(int resource, const sturct rlimit *rlim) : <sys/resource.h>
     *
     * @param resource 위 코드에서는 스택의 크기를 증가시키기 위해 RLIMIT_STACK 상수 사용
     * @param *rlim rlimit 구조체를 통해 제한된 리소스의 크기를 조회 또는 설정
     *
     * rlimit 구조체는 다음과 같은 구성을 가짐
     * ```
     * struct rlimit {
     *     rlim_t rlim_cur;  // Soft limit (프로세스에 할당하는 리소스의 제한 크기)
     *     rlim_t rlim_max;  // Hard limit (시스템에서 제공할 수 있는 리소스 최대 제한 크기)
     * };
     * ```
     *
     * rlim_max(하드 제한 크기)는 주로 getrlimit() 함수를 호출하여
     * 시스템에 설치된 하드웨어로 제공 가능한 크기를 알고자 할 때 사용
     */

    return (*env)->NewStringUTF(env, buffer);
}

JNIEXPORT jint JNICALL
Java_com_example_ndk_1sample_android_1process_MemoryOptimizationActivity_allocMemory(
        JNIEnv *env, jobject obj) {

    // buffer로 사용하는 메모리를 할당받음
    buffer = memalign(16, 100);

    if (NULL == buffer) {
        LOGE("Failed to allocate memory");
        return -1;
    }
    return 0;
}

JNIEXPORT jint JNICALL
Java_com_example_ndk_1sample_android_1process_MemoryOptimizationActivity_freeMemory(
        JNIEnv *env, jobject obj) {

    // 할당받은 메모리 해제
    if (buffer) free(buffer);
    buffer = NULL;

    return 0;
}