package com.example.ndk_sample.jni;

import android.util.Log;
import android.widget.TextView;

public class JNICallBackField {
    private String stringField;
    private static int intField;
    private static final String TAG = "JNICallBackField";

    static {
        System.loadLibrary("native-lib");
    }

    private native void fieldAccess();

    public JNICallBackField(String stringField, int intField) {
        this.stringField = stringField;
        this.intField =  intField;
    }

    public JNICallBackField() {
        this.stringField = "stringField";
        this.intField = 0;
    }

    public void doFieldAccess(TextView result) {
        fieldAccess();

        // LogCat에서 출력
        Log.i(TAG, "In Java");
        Log.i(TAG, " - stringField = " + stringField);
        Log.i(TAG, " - intField = " + intField);

        // View에서 출력
        result.setText("Field values converted from C to Java\n → " + stringField + " "+ intField);
    }
}
