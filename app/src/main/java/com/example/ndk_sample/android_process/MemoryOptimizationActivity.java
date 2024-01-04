package com.example.ndk_sample.android_process;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.example.ndk_sample.databinding.ActivityMemoryOptimizationBinding;


public class MemoryOptimizationActivity extends AppCompatActivity {
    private TextView tv;
    private Button btn;
    private int status;

    static {
        System.loadLibrary("native-lib");
    }

    ActivityMemoryOptimizationBinding binding;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMemoryOptimizationBinding.inflate(getLayoutInflater());
        View view = binding.getRoot();
        setContentView(view);

        tv = binding.tvResult;
        btn = binding.btnStart;

        btn.setOnClickListener(view1 -> {
            if (0 == status)
                tv.setText(stringFromJNI());
            else
                tv.setText("정상적으로 작업을 수행할 수 없습니다.");
        });
    }

    @Override
    protected void onStart() {
        super.onStart();

        // 필요한 메모리를 할당받음
        status = allocMemory();
        if (-1 == status) {
            Toast.makeText(this, "메모리를 할당받지 못했습니다.", Toast.LENGTH_SHORT).show();
        }
    }

    @Override
    protected void onStop() {
        super.onStop();

        // 할당받은 메모리 반환
        status = freeMemory();
    }

    public native String stringFromJNI();
    public native int allocMemory();
    public native int freeMemory();
}
