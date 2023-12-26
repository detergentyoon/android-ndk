package com.example.ndk_sample.jni;

import androidx.annotation.NonNull;

public class JNICallBackMethod {
    private int x, y, sum;
    static { // 공유 라이브러리를 메모리에 할당
        System.loadLibrary("native-lib");
    }

    // 네이티브 함수
    public native String PrinttoString();

    public JNICallBackMethod() {
        this.x = 0;
        this.y = 0;
        this.sum = 0;
    }

    public JNICallBackMethod(int x, int y) {
        this.x = x;
        this.y = y;
        this.sum = x + y;
    }

    @NonNull
    @Override
    public String toString() {
        return "JNICallBackMethod\n[ " + x + " + " + y + " = " + sum + " ]";
    }
}
