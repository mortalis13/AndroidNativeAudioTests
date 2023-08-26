package org.home.nativetests;

public class AudioPlayer {
  
  static {
    System.loadLibrary("nativetests");
  }
  
  
  public void setupAsyncAudio() {
    setupAsyncStream();
  }
  
  public void setupSyncAudio() {
    setupSyncStream();
  }
  
  public void testDynamicWave() {
    testAudioWave();
  }
  
  public void play(String audioPath) {
    playAudio(audioPath);
  }
  
  public void pause() {
    pauseAudio();
  }
  
  public void stop() {
    stopAudioStream();
  }
  
  private native boolean setupAsyncStream();
  private native boolean setupSyncStream();
  private native void testAudioWave();
  private native boolean stopAudioStream();
  
  private native boolean playAudio(String audioPath);
  private native void pauseAudio();
  
}
