package com.example.ndk_sample;

import android.content.Intent;
import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import com.example.ndk_sample.databinding.ActivityMainBinding;
import com.example.ndk_sample.jni.GetLineActivity;

public class MainActivity extends AppCompatActivity {

    // 응용 프로그램 시작 시 'native-lib' 라이브러리를 로드하는 데 사용됩니다.
    static {
        System.loadLibrary("native-lib");
    }

    private String[] items = {"Get Line"};

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

            Class<?> destinationActivity = null;
            if (items[0].equals(selectedItem)) {
                destinationActivity = GetLineActivity.class;
            }

            Intent it = new Intent(MainActivity.this, destinationActivity);

            it.putExtra("selectedItem", selectedItem);

            startActivity(it);
        });
    }

    /**
     * 'native-lib'이라는 이름을 가진 네이티브 라이브러리에 의해 구현되는 네이티브 메서드이며,
     * 이 라이브러리는 이 애플리케이션과 함께 패키지화되어 있습니다.
     */
    public native String stringFromJNI();
}