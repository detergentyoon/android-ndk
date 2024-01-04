package com.example.ndk_sample;

import android.content.Intent;
import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import com.example.ndk_sample.databinding.ActivityMainBinding;
import com.example.ndk_sample.jni.DynamicLinkerActivity;
import com.example.ndk_sample.jni.ExceptionHandlingActivity;
import com.example.ndk_sample.jni.GetLineActivity;
import com.example.ndk_sample.jni.JNICallBackArrayActivity;
import com.example.ndk_sample.jni.JNICallBackFieldActivity;
import com.example.ndk_sample.jni.JNICallBackMethodActivity;

public class MainActivity extends AppCompatActivity {

    // 응용 프로그램 시작 시 'native-lib' 라이브러리를 로드하는 데 사용
    static {
        System.loadLibrary("native-lib");
    }

    private final String[] items = {
            "GetLine",
            "JNICallBackMethod",
            "JNICallBackField",
            "JNICallBackArray",
            "ExceptionHandling",
            "DynamicLinker"
    };

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        TextView title = binding.sampleText;
        ListView listView = binding.listView;

        // 네이티브 메소드 호출
        title.setText(stringFromJNI());

        ArrayAdapter<String> adapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1, items);
        listView.setAdapter(adapter);

        listView.setOnItemClickListener((parent, view, position, id) -> {
            String selectedItem = (String) parent.getItemAtPosition(position);

            Class<?> destinationActivity;
            if (items[0].equals(selectedItem)) {
                destinationActivity = GetLineActivity.class;
            } else if (items[1].equals(selectedItem)) {
                destinationActivity = JNICallBackMethodActivity.class;
            } else if (items[2].equals(selectedItem)) {
                destinationActivity = JNICallBackFieldActivity.class;
            } else if (items[3].equals(selectedItem)) {
                destinationActivity = JNICallBackArrayActivity.class;
            } else if (items[4].equals(selectedItem)) {
                destinationActivity = ExceptionHandlingActivity.class;
            } else if (items[5].equals(selectedItem)) {
                destinationActivity = DynamicLinkerActivity.class;
            } else {
                return;
            }

            Intent it = new Intent(MainActivity.this, destinationActivity);
            it.putExtra("selectedItem", selectedItem);
            startActivity(it);
        });
    }

    /**
     * 'native-lib'이라는 이름을 가진 네이티브 라이브러리에 의해 구현되는 네이티브 메서드로,
     * 이 라이브러리는 이 애플리케이션과 함께 패키지화되어 있음
     */
    public native String stringFromJNI();
}