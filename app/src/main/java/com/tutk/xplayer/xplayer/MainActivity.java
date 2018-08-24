package com.tutk.xplayer.xplayer;

import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;

import java.io.File;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private Button mButton;
    private Button mBtnStream2File;
    private Button mBtnStream2Rtmp;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        //去掉标题栏
        supportRequestWindowFeature(Window.FEATURE_NO_TITLE);
        //全屏,隐藏状态
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        //屏幕为横屏
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

        setContentView(R.layout.activity_main);

        mButton = findViewById(R.id.open_button);
        mBtnStream2File = findViewById(R.id.btn_stream_to_file);
        mBtnStream2Rtmp = findViewById(R.id.btn_stream_to_rtmp);
        mButton.setOnClickListener(this);
        mBtnStream2File.setOnClickListener(this);
        mBtnStream2Rtmp.setOnClickListener(this);

    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.open_button:
                Log.e("XPlay", " buttononClick~~~");
                // 打开文件夹
                Intent intent = new Intent(MainActivity.this, OpenUrlActivity.class);
                startActivity(intent);
                break;
            case R.id.btn_stream_to_file:
                File sdPath = Environment.getExternalStorageDirectory();//获取跟目录
                if (mBtnStream2File.getText().equals("写入文件")) {
                    Log.e("XPlay", "开始写入文件");
                    mBtnStream2File.setText("停止写入");

                    startPushStream(sdPath.getAbsolutePath()+"/rtsptest.flv",false);
                } else {
                    Log.e("XPlay", "停止写入文件");
                    mBtnStream2File.setText("写入文件");
                    stopPushStream();
                    File f = new File(sdPath + "/rtsptest.flv");
                    if (f.exists() && f.isFile()){
                        Log.e("XPlay","" + f.length());
                    }else{
                        Log.e("XPlay","file doesn't exist or is not a file");
                    }

                }

                break;
            case R.id.btn_stream_to_rtmp:
                Log.e("XPlay", "开始推流");
                if (mBtnStream2Rtmp.getText().equals("推流rtmp")) {
                    mBtnStream2Rtmp.setText("停止推流");
                    startPushStream("",true);
                } else {
                    mBtnStream2Rtmp.setText("推流rtmp");
                    stopPushStream();
                }
                break;
        }
    }

    // native function
    public native void startPushStream(String url,boolean isPushToRTMP);

    public native void stopPushStream();



}

