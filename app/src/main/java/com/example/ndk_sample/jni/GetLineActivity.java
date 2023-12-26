package com.example.ndk_sample.jni;

import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.example.ndk_sample.databinding.ActivityGetLineBinding;

public class GetLineActivity extends AppCompatActivity {

    private ActivityGetLineBinding binding;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityGetLineBinding.inflate(getLayoutInflater());
        View view = binding.getRoot();
        setContentView(view);

        TextView tv = binding.sampleText;

        // 문자열과 정수를 네이티브 함수에 넘깁니다.
        String input = getLine("입력 값 : ", 100);
        tv.setText(input);
    }

    // 다음 문장으로 getLine() 메소드가 네이티브 함수라는 정보에 대해서 고지합니다.
    private native String getLine(String prompt, int value);

    static {
        System.loadLibrary("native-lib");
    }
}
