#ifndef FILE_PLAYER_H
#define FILE_PLAYER_H

#include <string>

#include "oboe/Oboe.h"

#include "AudioDecoder.h"
#include "defs.h"

using namespace std;
using namespace oboe;


class FilePlayer {

class MyDataCallback;
class MyErrorCallback;

public:
  FilePlayer();
  ~FilePlayer();
  
  bool setupAsyncStream();
  bool setupSyncStream();
  void testAudioWave();
  
  bool stop();
  bool close();
  
  bool play(string audioPath);
  void pause();
  

private:
  bool loadFile(string audioPath);
  void writeAudio(float* stream, int32_t numFrames);

  void emptyQueue();


private:
  static constexpr int kChannelCount = 2;

  AudioDecoder* decoder = NULL;
  SharedQueue dataQ;
  
  bool isPlaying = false;
  
  shared_ptr<AudioStream> audioStream;
  shared_ptr<AudioStreamWriter> streamWriter;
  shared_ptr<MyDataCallback> mDataCallback;
  shared_ptr<MyErrorCallback> mErrorCallback;


class MyDataCallback : public AudioStreamDataCallback {
public:
  MyDataCallback(FilePlayer *parent) : mParent(parent) {}
  DataCallbackResult onAudioReady(AudioStream *audioStream, void *audioData, int32_t numFrames) override;
private:
  FilePlayer *mParent;
};


class MyErrorCallback : public AudioStreamErrorCallback {
public:
  MyErrorCallback(FilePlayer *parent) : mParent(parent) {}
  virtual ~MyErrorCallback() {}
  void onErrorAfterClose(AudioStream *oboeStream, oboe::Result error) override;
private:
  FilePlayer *mParent;
};


class StreamWriter : public AudioStreamWriter {
public:
  StreamWriter(FilePlayer *parent) : mParent(parent) {}
  virtual ~StreamWriter() {}
  virtual void writeAudio(uint8_t* stream, int32_t numFrames);
private:
    FilePlayer *mParent;
};

};

#endif //FILE_PLAYER_H
