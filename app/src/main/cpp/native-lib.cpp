#include <jni.h>
#include <string>
#include <android/log.h>

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_ndk_1sample_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env -> NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_ndk_1sample_jni_GetLineActivity_getLine(
        JNIEnv *env,
        jobject /* this */,
        jstring prompt, /* Java로부터 받는 문자열 데이터 */
        int value      /* Java로부터 받는 int 데이터 */ ) {

    /**
     * GetStringUTFChars() 함수를 사용하여 Java의 문자열을
     * C/C++ 언어에서 사용할 수 있는 UTF-8 타입의 char 문자로 변경합니다.
     * 이 때 반환되는 문자열은 새롭게 생성한 힙 메모리에 담아 반환됩니다.
     *
     *
     * const jbyte * GetStringUTFChars(jstring string, jboolean *isCopy) : <jni.h>
     *
     *      Java의 유니코드 문자열을 UTF-8 문자열로 변환해주고,
     *      C/C++ 언어에서 문자열을 인식할 수 있도록 문자열 끝에 NULL을 추가시킵니다.
     *              ※ 만약 C 언어의 strcpy()를 사용하여 jstring 인수를 그대로 char 타입 변수에 복사하면,
     *                 jstring 타입의 문자열 끝에 NULL이 존재하지 않아 'out of memory' 에러가 발생하게 됩니다.
     *
     *      isCopy 인수는 정상적으로 작업이 완료되었을 때 JNI_TRUE를 반환받고,
     *      에러가 발생한 경우에는 JNI_FALSE를 반환받습니다.
     */
    const char *str = env -> GetStringUTFChars(prompt, NULL);

    if (str == NULL)
        return NULL;

    // 기본 int 타입 value는 변환 과정 없이 그대로 사용이 가능합니다.
    // 네이티브 함수로 받은 문자열과 int 타입의 정수를 Logcat 화면에 출력합니다.
    __android_log_print(ANDROID_LOG_INFO, "App Debug",  "%s(default %d) : ", str, value);

    /**
     * char 타입 변수에서 사용한 메모리를 ReleseStringUTFChars()를 통해 함수 반환 이전에 해제합니다.
     *
     * 네이티브 함수 내부에서 새롭게 메모리를 할당하였기 때문에
     * Java 프로그램으로 함수를 반환하기 전에 할당된 메모리를 해제시켜 주어야 합니다.
     *
     *
     * void ReleaseStringUTFChars(jstring string, const jbyte *utf) : <jni.h>
     *
     *      네이티브 함수의 제어를 JVM으로 반환하는 시점에 호출되는 함수로,
     *      GetStringUTFChars() 함수로부터 반환받는 버퍼를 해제시킬 때 사용합니다.
     *
     *
     * jstring 타입의 변수는 객체를 가리키는 것이 아닌 일반 문자열의 메모리 주소를 가리킵니다.
     * (Java 내에 존재하는 문자열은 객체로 존재)
     *
     * 네이티브 함수로 문자열을 전달해 주기 위해 Java 문자열 객체를 별도 문자열 buffer로 만들어 전달합니다.
     * 그렇기에 Java로부터 받은 문자열 buffer는 Java의 가비지 콜렉션의 대상 객체가 되지 않습니다.
     *
     * 따라서 ReleaseStringUTFChars() 함수는 내부에서 생성된
     * jbyte 타입의 문자열 buffer와 Java로부터 받은
     * jstring 타입의 문자열 모두를 삭제시키는 기능을 수행합니다.
     */
    env -> ReleaseStringUTFChars(prompt, str);

    const char *buf = "Hello JNI";
    return env -> NewStringUTF(buf); // Java에서 사용하는 UTF-16으로 변환하여 반환합니다.
}