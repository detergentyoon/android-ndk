package com.example.ndk_sample.jni;

public class ExceptionHandling {
    private static final String TAG = "ExceptionHandling";

    // 예외 클래스의 인스턴스를 생성하여 예외를 발생시키는 함수
    public native void nativeThrow() throws IllegalArgumentException;

    // Java 클래스 내에서 예외를 발생시키는 메소드를 호출하는 함수
    public native void nativeJavaThrow();

    static {
        System.loadLibrary("native-lib");
    }

    // 네이티브 함수 내부에서 예외를 발생시키는 메소드
    private void javaCatchThrow() throws NullPointerException {
        throw new NullPointerException("Null Pointer Exception thrown in javaCatchThrow");
    }
}