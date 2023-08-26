#define LOG_MODULE_NAME "OboePlayerJNI_"

#include <jni.h>

#include <string>

#include "utils/logging.h"
#include "FilePlayer.h"


#ifdef __cplusplus
extern "C" {
#endif

using namespace std;


static FilePlayer player;


JNIEXPORT jboolean JNICALL Java_org_home_nativetests_AudioPlayer_setupAsyncStream(JNIEnv *env, jclass obj) {
  LOGI(__func__);
  bool result = player.setupAsyncStream();
  return result;
}


JNIEXPORT jboolean JNICALL Java_org_home_nativetests_AudioPlayer_setupSyncStream(JNIEnv *env, jclass obj) {
  LOGI(__func__);
  bool result = player.setupSyncStream();
  return result;
}


JNIEXPORT void JNICALL Java_org_home_nativetests_AudioPlayer_testAudioWave(JNIEnv *env, jclass obj) {
  LOGI(__func__);
  player.testAudioWave();
}


JNIEXPORT jboolean JNICALL Java_org_home_nativetests_AudioPlayer_stopAudioStream(JNIEnv *env, jobject obj) {
  LOGI(__func__);
  bool result1 = player.stop();
  bool result2 = player.close();
  return result1 && result2;
}


JNIEXPORT jboolean JNICALL Java_org_home_nativetests_AudioPlayer_playAudio(JNIEnv *env, jclass obj, jstring jaudioPath) {
  LOGI(__func__);
  
  const char* audioPathBytes = env->GetStringUTFChars(jaudioPath, 0);
  string audioPath(audioPathBytes);
  env->ReleaseStringUTFChars(jaudioPath, audioPathBytes);
  
  bool result = player.play(audioPath);
  
  if (!result) {
    LOGE("Could not properly load audio file. Check the previous logs.");
  }
  
  return result;
}


JNIEXPORT void JNICALL Java_org_home_nativetests_AudioPlayer_pauseAudio(JNIEnv *env, jclass obj) {
  LOGI(__func__);
  player.pause();
}

#ifdef __cplusplus
}
#endif
