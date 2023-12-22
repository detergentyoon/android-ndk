package com.example.ndk_sample;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import com.example.ndk_sample.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // 응용 프로그램 시작 시 'native-lib' 라이브러리를 로드하는 데 사용됩니다.
    static {
        System.loadLibrary("native-lib");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // 네이티브 메소드 호출
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());
    }

    /**
     * 'native-lib'이라는 이름을 가진 네이티브 라이브러리에 의해 구현되는 네이티브 메서드이며,
     * 이 라이브러리는 이 애플리케이션과 함께 패키지화되어 있습니다.
     */
    public native String stringFromJNI();
}