package com.example.ndk_sample.android_process;

import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.example.ndk_sample.databinding.ActivityGetLineBinding;

public class GetPidActivity extends AppCompatActivity {
    ActivityGetLineBinding binding;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityGetLineBinding.inflate(getLayoutInflater());
        View view = binding.getRoot();
        setContentView(view);

        TextView tv = binding.sampleText;
        tv.setText(getPID());
    }

    public native String getPID();
}
