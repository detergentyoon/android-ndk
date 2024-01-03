package com.example.ndk_sample.jni;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.example.ndk_sample.databinding.ActivityExceptionHandlingBinding;

public class ExceptionHandlingActivity extends AppCompatActivity {
    private static final String TAG = "ExceptionHandling";

    private ActivityExceptionHandlingBinding binding;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityExceptionHandlingBinding.inflate(getLayoutInflater());
        View view = binding.getRoot();
        setContentView(view);

        Button btnCallException = binding.btnToast;
        btnCallException.setOnClickListener(saveListener);
    }

    private View.OnClickListener saveListener = view -> {
        ExceptionHandling ehandler = new ExceptionHandling();

        Log.i(TAG, "Native exception catch and throw test");
        try {
            ehandler.nativeThrow();
        } catch (IllegalArgumentException e) {
            Toast.makeText(this, e.getMessage(), Toast.LENGTH_LONG).show();
        }
        try {
            ehandler.nativeJavaThrow();
        } catch (Exception e) {
            // 예외 객체가 삭제되었기 때문에 아래의 Toast 메시지는 나타나지 않음
            Toast.makeText(this, e.getMessage(), Toast.LENGTH_LONG).show();
        }
    };
}
