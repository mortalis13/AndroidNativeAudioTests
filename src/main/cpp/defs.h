#ifndef DEFS_H
#define DEFS_H

#include "utils/LockFreeQueue.h"

typedef LockFreeQueue<float, 2 << 19> SharedQueue;

class AudioStreamWriter {
public:
  virtual void writeAudio(uint8_t* stream, int32_t numFrames) = 0;
};

#endif //DEFS_H