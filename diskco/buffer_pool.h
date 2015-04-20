#ifndef __Diskco__BufferPool__
#define __Diskco__BufferPool__
#include <stdio.h>
#include "buffer.h"
class BufferPool {
public:
  static BufferPool* instance();
  Buffer* get_buffer();
  void release_buffer(Buffer* buffer);
  BufferPool(int poolsize, int64_t buffer_size);  

private:
  static BufferPool* m_pInstance;
  Buffer** _pool;
  int _pool_size;
  int64_t _buffer_size;
  int _buffer_count;
};

#endif