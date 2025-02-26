package com.example.got_hook;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import com.example.got_hook.databinding.ActivityMainBinding;
import dalvik.system.PathClassLoader;

import android.util.Log;

public class MainActivity extends AppCompatActivity {

    static{
        System.loadLibrary("hello");
    }
    static{
        System.loadLibrary("got_hook");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
        PathClassLoader pathClassLoader = (PathClassLoader) getClassLoader();
        String nativeWritePath = pathClassLoader.findLibrary("hello");
        Log.d("native hook:","The path is :" + nativeWritePath);
        hello();
        hook(nativeWritePath);
        hello();
    }


    /**
     * A native method that is implemented by the 'got_hook' native library,
     * which is packaged with this application.
     */
    public native void hello();
    public native void hook(String filename);
}