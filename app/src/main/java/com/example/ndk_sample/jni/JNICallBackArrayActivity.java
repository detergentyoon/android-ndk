package com.example.ndk_sample.jni;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.example.ndk_sample.databinding.ActivityJniCallbackArrayBinding;

public class JNICallBackArrayActivity extends AppCompatActivity {
    private static final String TAG = "JNICallBackArray";

    ActivityJniCallbackArrayBinding binding;

    TextView result0;
    TextView result1;
    TextView result2;
    TextView result3;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityJniCallbackArrayBinding.inflate(getLayoutInflater());
        View view = binding.getRoot();
        setContentView(view);

        result0 = binding.tvResult0;
        result1 = binding.tvResult1;
        result2 = binding.tvResult2;
        result3 = binding.tvResult3;

        Button btn = binding.btnAccess;
        btn.setOnClickListener(saveListener);
    }

    private final View.OnClickListener saveListener = view -> {
        JNICallBackArray arr = new JNICallBackArray();
        arr.doArray(result0, result1, result2, result3);

        int i = 100;
        Log.i(TAG, "Native code constructed java.lang.Integer " + arr.getIntegerRef(i));
    };
}
