package com.example.ndk_sample.jni;

import android.util.Log;
import android.widget.TextView;

public class JNICallBackArray {
    static {
        System.loadLibrary("native-lib");
    }

    private native int [] intArrayAccess(int [] ai);
    private native int stringArrayAccess(String [] s);

    private static final String TAG = "JNICallBackArray";

    public void doArray(TextView result0, TextView result1,
                        TextView result2, TextView result3) {
        int i = 0;

        String s[] = new String[10];
        int   ai[] = new int[10];

        for (i = 0; i < 10; ++i) {  // 각 배열에 값 입력
            String fResult0 = "String: " + Integer.toString(i);

            s[i] = fResult0;
            // stringArrayAccess()에서 개별 문자열을 Logcat에 출력하는 코드가 여기에서 적용됨
            result0.append(fResult0 + "\n"); // View에 출력

            ai[i] = i;
        }

        // 네이티브 함수를 호출하는 동시에 String 객체 배열을 인수로 전달
        // stringArrayAccess() 함수는 작업한 String 배열의 크기를 반환
        String fResult1 = "String array size: " + stringArrayAccess(s);
        Log.i(TAG, fResult1);
        result1.setText(fResult1);

        String str = "";
        for (i = 0; i < 10; ++i) {
            if (i < 9)
                str += (ai[i] + ", ");
            else str += ai[i];
        }

        String fResult2 = "Original int array: { " + str + " }";
        Log.i(TAG, fResult2);
        result2.setText(fResult2);


        // 네이티브 함수를 호출하는 동시에 int 배열을 인수로 전달
        // intArrayAccess() 함수는 업데이트된 C/C++ 타입의 int[] 배열을 반환
        int ao[] = intArrayAccess(ai);

        // intArrayAccess() 함수로부터 반환받은 int 배열을 LogCat에 출력
        str = "";
        for (i = 0; i < 10; ++i) {
            if (i < 9)
                str += (ao[i] + ", ");
            else str += ao[i];
        }

        String fResult3 = "Modified int array: { " + str + " }";
        // intArrayAccess()의 return 값인 ret 배열
        Log.i(TAG, fResult3);
        result3.setText(fResult3);
    }
}
