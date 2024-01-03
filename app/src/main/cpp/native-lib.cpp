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

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_ndk_1sample_jni_JNICallBackMethod_PrinttoString(
        JNIEnv *env, jobject obj) {

    // obj 인스턴스를 사용하여 JNICallBackMethod를 얻음
    jclass cls = env->GetObjectClass(obj);

    // toString() 메소드의 ID를 얻음
    jmethodID mid = env->GetMethodID(cls, "toString", "()Ljava/lang/String;");

    // 메소드 ID를 사용하여 함수 호출 및 Java 문자열 반환
//    jstring s = (jstring) env->CallNonvirtualObjectMethod(obj, cls, mid);
    jstring s = (jstring) env->CallObjectMethod(obj, mid);

    // C 언어 타입의 문자로 변경하기 위해 NULL 추가
    const char *buf = env->GetStringUTFChars(s, 0);

    // buf의 내용을 LogCat 화면에 출력
    __android_log_print(ANDROID_LOG_INFO, "JNICallBackMethod_PrinttoString", "%s", buf);

    // 사용한 버퍼 삭제
    env->ReleaseStringUTFChars(s, buf);

    // toString() 함수로부터 반환받은 문자열 반환
    return s;
}

// Java 문자열을 logCat에 출력하는 함수
static void printStringLogcatField(
        JNIEnv *env, jstring jstr) {

    // Java 문자열을 C 언어 형식(utf-8)로 변환
    const char * str = env->GetStringUTFChars(jstr, 0);

    __android_log_print(ANDROID_LOG_INFO, "JNICallBackField_fieldAccess",
                        "stringField = %s\n", str);

    // GetStringUTFChars를 사용할 때 할당된 메모리 공간을 해제
    env->ReleaseStringUTFChars(jstr, str);
}

extern "C" JNIEXPORT void JNICALL
/**
 * Java Class 필드에 접근하여
 * 입력된 String 필드 값과 "Hello " 문자열을 병합시키고,
 * 입력된 static int 필드 값에 +10 처리를 수행하는 네이티브 함수
 */
Java_com_example_ndk_1sample_jni_JNICallBackField_fieldAccess(
        JNIEnv *env, jobject obj) {

    // 인스턴스로부터 클래스 획득
    jclass cls = env->GetObjectClass(obj);

    // intField 정적 필드 아이디 획득
    jfieldID intFieldId = env->GetStaticFieldID(cls, "intField", "I");
    if (!intFieldId) {
        __android_log_print(ANDROID_LOG_INFO, "JNICallBackField_fieldAccess",
                            "Error getting fid for intField\n");
        return;
    }

    // 필드 아이디를 통해 static 필드 내에 할당된 데이터(number 입력 값)를 읽음
    jint existingInt = env->GetStaticIntField(cls, intFieldId);

    __android_log_print(ANDROID_LOG_INFO, "JNICallBackField_fieldAccess",
                        "intField = %d\n", existingInt);

    // static int 필드 값에 +10 처리
    env->SetStaticIntField(cls, intFieldId, existingInt + 10);


    // Java 클래스 내의 stringField 아이디 획득
    jfieldID strFieldId = env->GetFieldID(cls, "stringField", "Ljava/lang/String;");
    if (!strFieldId) {
        __android_log_print(ANDROID_LOG_INFO, "JNICallBackField_fieldAccess",
                            "Error getting fid for stringField\n");
        return;
    }

    // 필드 아이디를 통해 String 필드 내에 할당된 데이터(text 입력 값)를 읽음
    jstring existingStr = (jstring) env->GetObjectField(obj, strFieldId);

    // Logcat에 Java 문자열 출력
    printStringLogcatField(env, existingStr);

    // utf-8 타입으로 변수에 입력할 새로운 문자 생성
    const char *appendStr = "Hello ";

    // 새로운 문자열과 기존 문자열을 병합하여 새로운 Java 문자열 생성
    const char *existingStrChars = env->GetStringUTFChars(existingStr, 0);
    std::string mergedStr = std::string(appendStr) + existingStrChars;
    env->ReleaseStringUTFChars(existingStr, existingStrChars);

    jstring jstr = env->NewStringUTF(mergedStr.c_str());

    // Logcat에 Java 문자열 출력
    printStringLogcatField(env, jstr);

    // 새로운 Java 문자열을 stringField에 할당
    env->SetObjectField(obj, strFieldId, jstr);
}


static void printStringLogcatArray(
        JNIEnv *env, jstring jstr) {

    // Java 문자열을 C 언어 형식(utf-8)로 변환
    const char * str = env->GetStringUTFChars(jstr, 0);

    __android_log_print(ANDROID_LOG_INFO, "JNICallBackArray","%s\n", str);

    // GetStringUTFChars를 사용할 때 할당된 메모리 공간을 해제
    env->ReleaseStringUTFChars(jstr, str);
}

/**
 * Java에서 전달받은 String[]의 원소들을 개별 문자열로
 * Logcat에 출력하고, 배열의 크기를 반환하는 함수
 */
extern "C" JNIEXPORT jint JNICALL
Java_com_example_ndk_1sample_jni_JNICallBackArray_stringArrayAccess(
        JNIEnv *env, jobject obj, jobjectArray as) {

    int i;
    jsize len = env->GetArrayLength(as); // 배열 크기 반환

    for (i = 0; i < len; ++i) {
        // 배열로부터 각 요소마다 개별 문자열 생성
        jstring jstr = (jstring) env->GetObjectArrayElement(as, i);
        printStringLogcatArray(env, jstr);  // Logcat에 출력
        env->DeleteLocalRef(jstr);  // 사용한 문자열의 메모리 회수
    }

    return len;
}

extern "C" JNIEXPORT jintArray JNICALL
/**
 * Java에서 전달받은 int[]의 각 원소에 대해 *10 연산하고
 * 값이 업데이트된 배열을 반환하는 함수
 */
Java_com_example_ndk_1sample_jni_JNICallBackArray_intArrayAccess(
        JNIEnv *env, jobject obj, jintArray ai) {

    int i, sum = 0;
    jsize len = env->GetArrayLength(ai);  // 배열 크기 반환

    // 전달받은 Java int[] 배열을 네이티브 int 배열로 변환
    jint *body = env->GetIntArrayElements(ai, 0);

    // 각 네이티브 배열 요소를 순회하며 값 합산
    for (i = 0; i < len; ++i) {
        sum += body[i];
    }
    // 합산 결과를 Logcat에 출력
    __android_log_print(ANDROID_LOG_INFO, "JNICallBackArray", "sum = %d\n", sum);

    jintArray ret = env->NewIntArray(len); // 새로운 Java int[] 배열 생성

    // Java int[] 배열을 네이티브 int 배열로 변환
    jint *retarray = env->GetIntArrayElements(ret, 0);

    // 각 네이티브 배열 요소를 순회하며 값 * 10 곱연산
    for (i = 0; i < len; ++i) {
        retarray[i] = body[i] * 10;
    }

    // 작업한 네이티브 배열을 Java 배열로 변환하고 메모리 삭제
    env->ReleaseIntArrayElements(ai, body, JNI_ABORT);
    env->ReleaseIntArrayElements(ret, retarray, 0);
    /**
     * @param 0             elems 버퍼의 내용(업데이트 내용)을 원본 배열에 반영하고 메모리는 회수함
     * @param JNI_COMMIT    elems 버퍼의 내용(업데이트 내용)을 원본 배열에 반영하지만 메모리는 회수하지 않음
     * @param JNI_ABORT     elems 버퍼의 메모리를 회수하지만 elems 버퍼의 내용은 다시 복사되지 않고 버퍼와 함께 삭제됨
     *                          -> 원본 배열에 업데이트 내용을 반영하지 않고 메모리는 회수함
     *                             (복사본으로 작업하고 배열 수정은 없는 경우 유용)
     */

    return ret;
}



/**
 * (1) Integer 클래스의 인스턴스를 생성하고,
 * (2) Integer 클래스 내의 toString() 메소드를 사용하여
 * String 객체를 통해 메소드 내부에서 사용하는 함수
 *
 *  - String 문자열은 ReleaseStringUTFChar() 함수를 통해 문자열 메모리를 삭제시키고,
 *  - Integer 클래스의 인스턴스는 Java 힙 메모리에 생성되어 pass-by-reference 형태로 Java에 반환된다.
 *  - newojb 변수는 지역 변수이므로 함수가 반환될 때 자동으로 삭제된다.
 *  - 최종적으로 MainActivity 클래스에서 사용이 완료되면 Integer 객체는 가비지 콜렉션에 의해 자동으로 삭제된다.
 *
 * @return 메소드 내부에서 생성한 객체
 */
jobject getInteger(JNIEnv *env, jobject obj, jint number) {
    // java.lang.Integer 클래스 반환
    jclass cls = env->FindClass("java/lang/Integer");

    // Integer 클래스 생성자 ID 반환
    //      생성자의 아이디를 찾을 때에는 함수 이름을 <init>으로 사용
    //      생성자는 선언 시 반환하는 데이터 타입이 없으므로 V 식별자 (void) 사용
    jmethodID midInit = env->GetMethodID(cls, "<init>", "(I)V");
    if (NULL == midInit) return NULL;

    // 생성자와 초기화 데이터(네이티브 함수 호출 시 전달받는 인수)를 통해 클래스의 인스턴스 생성
    jobject newObj = env->NewObject(cls, midInit, number);

    // Integer 클래스 내의 toString() 메소드 ID 획득
    jmethodID stringID = env->GetMethodID(cls, "toString", "()Ljava/lang/String;");
    if (NULL == stringID) return NULL;

    // toString() 메소드 실행
    jstring result = (jstring) env->CallObjectMethod(newObj, stringID);
    const char *res = env->GetStringUTFChars(result, NULL);

    // Integer 클래스의 toString() 메소드 호출 결과를 Logcat에 출력
    __android_log_print(ANDROID_LOG_INFO, "JNICallBackArray",
                        "In C: the number is %s\n", res);
    env->ReleaseStringUTFChars(result, res); // 메모리 해제

    return newObj;
}

extern "C" JNIEXPORT jobject JNICALL
Java_com_example_ndk_1sample_jni_JNICallBackArray_getInteger(
        JNIEnv *env, jobject obj, jint number) {
    return getInteger(env, obj, number);
}

// Java의 객체를 가리키는 reference는 C/C++ 언어에서 사용하는 포인터와 기능적으로 다르다.
// JNI로 제공되는 참조 변수는 모두 지역 변수로 제공되며, 이는 스택에 저장되는 변수이기 때문에
// 함수가 반환되면 지역 변수는 자동으로 스택에서 삭제되는 특징을 갖는다.
//
// 위 함수에서는 객체를 대상으로 다루었으나, 객체(jobject) 타입을 포함한
// 클래스(jclass) 타입이나 배열(jarray) 또한 동일하게 적용된다.
//
// 하지만 jstring 타입의 경우 String 클래스로부터 생성되지만 객체가 아닌 문자열로 취급된다.
// 따라서 GetStringUTFChars() 함수나 GetByteArrayElement() 함수에 의해 반환되는 포인터는
// 클래스의 인스턴스나 객체가 아닌 단순 문자열로서 제공된다.
// 때문에 함수가 반환될 때 free() 함수를 호출하듯
// ReleaseStringUTFChars() 함수를 호출해 메모리를 해제해야 한다.


static jclass    classInteger;
static jobject   newObj;
static jmethodID integerInit;

/**
 * 스택에 저장된 참조 변수의 데이터가 함수의 반환과 동시에 삭제되지 않도록 하기 위해서
 * 지역 참조 변수를 전역 참조 변수로 변환(혹은 static 참조 변수)하여 사용한다.
 *
 * 이 함수에서는 생성한 객체를 네이티브 코드의 내부 함수에서 공동으로 사용하게끔 하기 위해
 * NewGlobalRef() 함수를 통해 지역 참조 변수를 전역 참조 변수로 변환하여 변수에 할당한다.
 */
jobject getIntegerRef(JNIEnv *env, jobject obj, jint number) {
    // 클래스를 지역 참조 변수로 변환
    jclass classIntegerLocal = env->FindClass("java/lang/Integer");

    // 지역 참조 변수를 전역 참조 변수로 변환
    classInteger = reinterpret_cast<jclass>(env->NewGlobalRef(classIntegerLocal));
    if (NULL == classInteger) return NULL;

    // 지역 참조 변수에 의해 사용된 메모리를 삭제 (아래 함수는 호출하지 않아도 무방함)
    env->DeleteLocalRef(classIntegerLocal);

    // 메소드 ID는 객체가 아니므로 NewGlobalRef() 함수 사용 X
    integerInit = env->GetMethodID(classInteger, "<init>", "(I)V");
    if (NULL == integerInit) return NULL;
    
    // Integer 클래스의 생성자를 사용해 인스턴스 생성 후 전역 변수로 변환
    newObj = env->NewGlobalRef(env->NewObject(classInteger, integerInit, number));
    
    // Integer 클래스 내에 존재하는 toString() 메소드의 ID 획득
    jmethodID stringID = env->GetMethodID(classInteger, "toString", "()Ljava/lang/String;");
    if (NULL == stringID) return NULL;
    
    // toString() 메소드 실행
    jstring result = (jstring)env->CallObjectMethod(newObj, stringID);
    const char *res = env->GetStringUTFChars(result, NULL);
    __android_log_print(ANDROID_LOG_INFO, "JNICallBackArray",
                        "In C: the number is %s\n", res);
    env->ReleaseStringUTFChars(result, res); // 사용한 메모리 해제

    return newObj;
}

extern "C" JNIEXPORT jobject JNICALL
Java_com_example_ndk_1sample_jni_JNICallBackArray_getIntegerRef(
        JNIEnv *env, jobject obj, jint number) {
    return getIntegerRef(env, obj, number);
}




extern "C" JNIEXPORT void JNICALL
Java_com_example_ndk_1sample_jni_ExceptionHandling_nativeThrow(
        JNIEnv *env, jobject obj) {
    jclass newException = env->FindClass("java/lang/IllegalArgumentException");

    if (newException == nullptr) {
        __android_log_print(ANDROID_LOG_INFO, "ExceptionHandling",
                            "Error finding the new exception class\n");
        return;
    }

    env->ThrowNew(newException, "IllegalArgumentException thrown from C++ code");
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_ndk_1sample_jni_ExceptionHandling_nativeJavaThrow(
        JNIEnv *env, jobject obj) {
    jthrowable exc;
    jclass cls = env->GetObjectClass(obj);

    jmethodID mid = env->GetMethodID(cls, "javaCatchThrow", "()V");

    if (mid == nullptr) {
        __android_log_print(ANDROID_LOG_INFO, "ExceptionHandling",
                            "Error obtaining method id\n");
        return;
    }

    // 발생한 예외 내용을 Logcat에 출력하고 예외 발생 사항에 대해서는 무시함
    env->CallVoidMethod(obj, mid);
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
 }