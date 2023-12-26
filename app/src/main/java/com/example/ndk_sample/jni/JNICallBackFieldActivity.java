package com.example.ndk_sample.jni;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.example.ndk_sample.databinding.ActivityJniCallbackMethodBinding;

public class JNICallBackFieldActivity extends AppCompatActivity {
    public int i = 0, j = 0;

    private static final String TAG = "JNICallBackFieldActivity";
    private ActivityJniCallbackMethodBinding binding;

    EditText ei;
    EditText ej;

    TextView tv;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityJniCallbackMethodBinding.inflate(getLayoutInflater());
        View view = binding.getRoot();
        setContentView(view);

        ei = binding.i;
        ej = binding.j;

        tv = binding.result;

        Button btn = binding.sum;
        btn.setOnClickListener(saveListener);
    }

    private final View.OnClickListener saveListener = view -> {
        try {
            i = Integer.parseInt(ei.getText().toString());
        } catch (NumberFormatException e) {
            i = 0; // 숫자가 아닌 문자가 포함되어 있는 경우
        }

        try {
            j = Integer.parseInt(ej.getText().toString());
        } catch (NumberFormatException e) {
            j = 0;
        }

        // JNICallBackMethod 클래스의 인스턴스 생성
        JNICallBackMethod m = new JNICallBackMethod(i, j);

        // 네이티브 함수 호출
        String s = "return method " + m.PrinttoString();

        Log.v(TAG, s);
        tv.setText(s);
    };
}
