/*
Copyright 2026 Maximilian Schmid

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in
  the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
“AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <ringbuffer/ringbuffer.h>

bool ringbuffer_init(ringbuffer_t *ringbuffer, void *buffer, size_t buffersize)
{
  if(NULL == ringbuffer ||
    NULL == buffer ||
    2 > buffersize)
  {
    return false;
  }

  ringbuffer->buffer = buffer;
  ringbuffer->buffer_size = buffersize;
  atomic_store(&ringbuffer->write_index, 0);
  atomic_store(&ringbuffer->read_index, 0);
  return true;
}

bool ringbuffer_empty(const ringbuffer_t *ringbuffer)
{
  size_t write_index, read_index;
  write_index = atomic_load(&ringbuffer->write_index);
  read_index = atomic_load(&ringbuffer->read_index);
  return write_index == read_index;
}

size_t ringbuffer_capacity(const ringbuffer_t *ringbuffer)
{
  const size_t write_index = atomic_load(&ringbuffer->write_index);
  const size_t read_index = atomic_load(&ringbuffer->read_index);
  const size_t buffer_size = ringbuffer->buffer_size;
  if(read_index == write_index)
  {
    return buffer_size - 1;
  }
  else if(read_index > write_index)
  {
    return read_index - write_index - sizeof(uint8_t); // one slot to distinguish empty and full
  }
  else
  {
    // with overflow
    return read_index + (buffer_size - write_index) - sizeof(uint8_t); // one slot to distinguish empty and full
  }
}

size_t ringbuffer_available(const ringbuffer_t *ringbuffer)
{
  const size_t write_index = atomic_load(&ringbuffer->write_index);
  const size_t read_index = atomic_load(&ringbuffer->read_index);
  const size_t buffersize = ringbuffer->buffer_size;

  if(write_index == read_index)
  {
    // ringbuffer is empty
    return 0;
  }
  else if(write_index > read_index)
  {
    return write_index - read_index;
  }
  else
  {
    return buffersize - read_index + write_index;
  }
}

bool ringbuffer_add_data(ringbuffer_t *ringbuffer, const void *data, size_t length)
{  
  // check for NULL pointers
  if(NULL == ringbuffer ||
     NULL == data)
  {
    return false;
  }

  uint8_t* byte_ptr = (uint8_t*)ringbuffer->buffer;
  const uint8_t* data_ptr = (const uint8_t*)data;
  size_t current_index = atomic_load(&ringbuffer->write_index);
  const size_t buffer_size = ringbuffer->buffer_size;

  size_t current_capacity = ringbuffer_capacity(ringbuffer);

  

  if(length > current_capacity)
  {
    // do not write anything when not enough space is free
    return false;
  }  

  for(size_t i=0; i<length; ++i)
  {
    byte_ptr[current_index] = data_ptr[i];
    current_index++;
    if(buffer_size <= current_index)
    {
      // overflow to zero when current index is equal to the buffersize
      current_index = 0;
    }
  }
  // ringbuffer->write_index = current_index;
  atomic_store(&ringbuffer->write_index, current_index);
  return true;
}

size_t ringbuffer_get_data(ringbuffer_t *ringbuffer, void *data, size_t buffer_size)
{
  uint8_t* byte_ptr = (uint8_t*)ringbuffer->buffer;
  uint8_t* data_ptr = (uint8_t*)data;

  size_t current_index = atomic_load(&ringbuffer->read_index);

  size_t buffersize = ringbuffer->buffer_size;

  size_t available = ringbuffer_available(ringbuffer);

  size_t i = 0;

  for(i=0; i<buffer_size && i<available; ++i)
  {
    data_ptr[i] = byte_ptr[current_index];

    current_index++;

    if(buffersize <= current_index)
    {
      // overflow -> start at zero
      current_index = 0;
    }
  }
  atomic_store(&ringbuffer->read_index, current_index);
  return i;
}