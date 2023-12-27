package com.example.ndk_sample.jni;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.example.ndk_sample.R;
import com.example.ndk_sample.databinding.ActivityJniCallbackFieldBinding;

public class JNICallBackFieldActivity extends AppCompatActivity {
    public String name;
    public int age;
    
    private ActivityJniCallbackFieldBinding binding;

    EditText etI;
    EditText etStr;

    TextView tvResult;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityJniCallbackFieldBinding.inflate(getLayoutInflater());
        View view = binding.getRoot();
        setContentView(view);
        
        etStr = binding.etName;
        etI = binding.etAge;

        tvResult = binding.tvResult;

        Button btn = binding.btnSum;
        btn.setOnClickListener(saveListener);
    }

    private final View.OnClickListener saveListener = view -> {
        try {
            name = etStr.getText().toString();
            age = Integer.parseInt(etI.getText().toString());

            // name이 빈 값으로 들어오지 않았고,
            // NumberFormatException이 발생하지 않은 경우에만 수행
            if (!name.equals("")) {
                JNICallBackField f = new JNICallBackField(name, age);
                f.doFieldAccess(tvResult);  // 네이티브 함수 fieldAccess() 호출
            } else {
                Toast.makeText(this, R.string.please_fill_in_the_input_field, Toast.LENGTH_SHORT).show();
            }
        } catch (NumberFormatException e) {
            Toast.makeText(this, R.string.please_fill_in_the_input_field, Toast.LENGTH_SHORT).show();
        }
    };
}
