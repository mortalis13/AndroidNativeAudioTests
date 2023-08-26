package org.home.nativetests;

import android.content.Context;
import android.os.Bundle;
import android.widget.Button;
import android.view.MotionEvent;
import android.Manifest;
import android.net.Uri;
import android.os.Environment;
import android.provider.Settings;
import android.content.Intent;
import android.os.Build;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.ContextCompat;

import static org.home.nativetests.Fun.log;
import static org.home.nativetests.Fun.logd;


public class MainActivity extends AppCompatActivity {

  private AudioPlayer audioPlayer;
  
  private Context context;
  
  private Button bTestSync;
  private Button bTestAsync;
  private Button bTestWave;
  
  private String audioPath = "/storage/emulated/0/_tests/loop.mp3";

  
  @Override
  protected void onCreate(Bundle savedInstanceState) {
    logd("MainActivity.onCreate()");
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);
    
    context = this;
    requestAppPermissions(context);
    
    init();
    configUI();
  }
  
  @Override
  protected void onDestroy() {
    logd("MainActivity.onDestroy()");
    super.onDestroy();
    if (audioPlayer != null) audioPlayer.stop();
  }
  
  protected void onStart() {
    logd("MainActivity.onStart()");
    super.onStart();
  }
  protected void onStop() {
    logd("MainActivity.onStop()");
    super.onStop();
  }
  protected void onRestart() {
    logd("MainActivity.onRestart()");
    super.onRestart();
  }
  protected void onPause() {
    logd("MainActivity.onPause()");
    super.onPause();
  }
  protected void onResume() {
    logd("MainActivity.onResume()");
    super.onResume();
  }
  
  @Override
  public void onBackPressed() {
    finishAndRemoveTask();
  }

  private void requestAppPermissions(Context context) {
    if (Build.VERSION.SDK_INT < 30) {
      if (ContextCompat.checkSelfPermission(context, Manifest.permission.WRITE_EXTERNAL_STORAGE) == 0) return;
      requestPermissions(new String[] {Manifest.permission.WRITE_EXTERNAL_STORAGE}, Vars.APP_PERMISSION_REQUEST_ACCESS_EXTERNAL_STORAGE);
      return;
    }
    if (Environment.isExternalStorageManager()) return;
    startActivity(new Intent(Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION, Uri.parse("package:" + BuildConfig.APPLICATION_ID)));
  }
  
  private void init() {
    
  }
  
  private void configUI() {
    bTestSync = findViewById(R.id.bTestSync);
    bTestAsync = findViewById(R.id.bTestAsync);
    bTestWave = findViewById(R.id.bTestWave);
    
    bTestSync.setOnClickListener(v -> {
      audioPlayer = new AudioPlayer();
      audioPlayer.stop();
      audioPlayer.setupSyncAudio();
      audioPlayer.play(audioPath);
    });
    
    bTestAsync.setOnClickListener(v -> {
      audioPlayer = new AudioPlayer();
      audioPlayer.stop();
      audioPlayer.setupAsyncAudio();
      audioPlayer.play(audioPath);
    });
    
    bTestWave.setOnClickListener(v -> {
      audioPlayer = new AudioPlayer();
      audioPlayer.stop();
      audioPlayer.testDynamicWave();
    });
  }

}
