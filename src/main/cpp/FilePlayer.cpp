#define LOG_MODULE_NAME "FilePlayer_"

#include "FilePlayer.h"

#include "utils/logging.h"


FilePlayer::FilePlayer() {
  streamWriter = make_shared<StreamWriter>(this);
}

FilePlayer::~FilePlayer() {
  if (decoder != NULL) delete decoder;
}


bool FilePlayer::setupSyncStream() {
  isPlaying = false;
  
  AudioStreamBuilder builder;
  builder.setFormat(AudioFormat::Float);
  builder.setChannelCount(kChannelCount);
  builder.setSampleRate(48000);

  auto result = builder.openStream(audioStream);
  if (result != Result::OK) {
    LOGE("Failed to open stream. Error: %s", convertToText(result));
    return false;
  }
  
  result = audioStream->requestStart();
  if (result != Result::OK) {
    LOGE("Failed to start stream. Error: %s", convertToText(result));
    return false;
  }
  
  if (this->decoder != NULL) {
    this->decoder->stop();
    delete this->decoder;
  }
  this->decoder = new AudioDecoder(streamWriter);
  this->decoder->setChannelCount(audioStream->getChannelCount());
  this->decoder->setSampleRate(audioStream->getSampleRate());
  
  return true;
}


bool FilePlayer::setupAsyncStream() {
  isPlaying = false;
  
  mDataCallback = make_shared<MyDataCallback>(this);
  mErrorCallback = make_shared<MyErrorCallback>(this);

  AudioStreamBuilder builder;
  builder.setSharingMode(SharingMode::Exclusive);
  builder.setPerformanceMode(PerformanceMode::LowLatency);
  builder.setFormat(AudioFormat::Float);
  builder.setFormatConversionAllowed(true);
  builder.setChannelCount(kChannelCount);
  builder.setDataCallback(mDataCallback);
  builder.setErrorCallback(mErrorCallback);
  builder.setSampleRate(48000);
  builder.setSampleRateConversionQuality(SampleRateConversionQuality::Medium);

  auto result = builder.openStream(audioStream);
  if (result != Result::OK) {
    LOGE("Failed to open stream. Error: %s", convertToText(result));
    return false;
  }
  
  result = audioStream->requestStart();
  if (result != Result::OK) {
    LOGE("Failed to start stream. Error: %s", convertToText(result));
    return false;
  }
  
  if (this->decoder != NULL) {
    this->decoder->stop();
    delete this->decoder;
  }
  this->decoder = new AudioDecoder(&dataQ);
  this->decoder->setChannelCount(audioStream->getChannelCount());
  this->decoder->setSampleRate(audioStream->getSampleRate());
  
  return true;
}


void FilePlayer::testAudioWave() {
  int channels = 2;
  int sample_rate = 44100;
  int duration = 1;
  
  shared_ptr<AudioStream> stream;
  
  AudioStreamBuilder builder;
  builder.setFormat(AudioFormat::Float);
  builder.setChannelCount(channels);
  builder.setSampleRate(sample_rate);
  
  builder.openStream(stream);
  stream->requestStart();
  
  // Data
  int total_samples = duration * sample_rate * channels;
  int numFrames = total_samples / channels;
  
  // == Byte buffer
  // int size = total_samples * sizeof(float);
  // uint8_t buffer[size];
  // for (int i = 0; i < size; i++) {
  //   buffer[i] = rand() % 255;
  // }
  
  // == Sine wave
  // float buffer[total_samples];
  // for (int i = 0; i < total_samples; i++) {
  //   int frame_id = i / 2;
  //   buffer[i] = sin(2 * M_PI * 440 * frame_id / sample_rate);
  // }
  
  // == Stereo white noise
  float buffer[total_samples];
  for (int i = 0; i < total_samples; i++) {
    buffer[i] = drand48();
  }

  stream->write(buffer, numFrames, 100);
  
  stream->requestStop();
  stream->close();
}


bool FilePlayer::stop() {
  if (!audioStream) return false;
  this->isPlaying = false;
  if (this->decoder) this->decoder->stop();
  auto result = audioStream->requestStop();
  return result == oboe::Result::OK;
}

bool FilePlayer::close() {
  if (!audioStream) return false;
  auto result = audioStream->close();
  return result == oboe::Result::OK;
}


bool FilePlayer::play(string audioPath) {
  this->isPlaying = false;
  this->decoder->stop();
  this->emptyQueue();
  
  bool result = loadFile(audioPath);
  if (!result) return result;
  
  this->isPlaying = true;
  
  this->decoder->start();
  LOGI("Decoder started");

  return true;
}

void FilePlayer::pause() {
  this->isPlaying = false;
}


void FilePlayer::emptyQueue() {
  float sample;
  while (this->dataQ.pop(sample)) {}
  LOGI("Queue emptied");
}


bool FilePlayer::loadFile(string audioPath) {
  int result = this->decoder->loadFile(audioPath);
  if (result < 0) return false;
  return true;
}


void FilePlayer::writeAudio(float* stream, int32_t numFrames) {
  // Audio thread
  for (int i = 0; i < numFrames; i++) {
    for (int ch = 0; ch < kChannelCount; ch++) {
      float sample = 0;
      if (this->isPlaying) {
        this->dataQ.pop(sample);
      }

      *stream++ = sample;
    }
  }
}


DataCallbackResult FilePlayer::MyDataCallback::onAudioReady(AudioStream* audioStream, void* audioData, int32_t numFrames) {
  if (!mParent->isPlaying) {
    memset(audioData, 0, numFrames * kChannelCount * sizeof(float));
    return DataCallbackResult::Continue;
  }
  
  float* stream = (float*) audioData;
  mParent->writeAudio(stream, numFrames);
  
  return DataCallbackResult::Continue;
}


void FilePlayer::MyErrorCallback::onErrorAfterClose(AudioStream* oboeStream, oboe::Result error) {
  LOGE("%s() - error = %s", __func__, oboe::convertToText(error));
  mParent->setupAsyncStream();
}


void FilePlayer::StreamWriter::writeAudio(uint8_t* stream, int32_t numFrames) {
  // Sync write
  auto result = mParent->audioStream->write(stream, numFrames, 100);
}
