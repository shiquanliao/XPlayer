package com.tutk.xplayer.xplayer;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

/**
 * @author : liaoshiquan
 * @date : 2018/8/12
 * @description : TODO
 */
public class OpenUrlActivity extends AppCompatActivity {
    private Button btnFile;
    private Button btnRtmp;
    private EditText mETFile;
    private EditText mETRtmp;
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.openurl);

        btnFile = findViewById(R.id.playvideo);
        btnRtmp = findViewById(R.id.playrtmp);
        mETFile = findViewById(R.id.fileurl);
        mETRtmp = findViewById(R.id.rtmpurl);

        btnFile.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Open(mETFile.getText().toString());
                finish();
            }
        });
        btnRtmp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Open(mETRtmp.getText().toString());
                finish();
            }
        });
    }


    // native method
    public native void Open(String url);

}
