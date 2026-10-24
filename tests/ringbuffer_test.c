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

#include <stdio.h>

#include <ringbuffer/ringbuffer.h>
#include <string.h>

int main()
{
  uint8_t buffer[128];
  uint8_t buffer2[128];
  uint8_t buffer3[128];
  size_t read = 0;
  ringbuffer_t ringbuffer;

  ringbuffer_init(&ringbuffer, buffer, sizeof(buffer));

  ringbuffer.write_index = 125;
  ringbuffer.read_index = 125;

  if(!ringbuffer_add_data(&ringbuffer, "hello, world", strlen("hello, world")))
  {
    return -1;
  }
  if(!ringbuffer_add_data(&ringbuffer, "abc", strlen("abc")))
  {
    return -1;
  }
  memset(buffer2, 0, sizeof(buffer2));
  
  read = ringbuffer_get_data(&ringbuffer, buffer2, sizeof(buffer2));
  printf("%s\n", buffer2);

  memset(buffer2, 0, sizeof(buffer2));

  read = ringbuffer_get_data(&ringbuffer, buffer2, sizeof(buffer2));
  printf("%s\n", buffer2);

  ringbuffer_init(&ringbuffer, buffer, sizeof(buffer));

  for(size_t i=0; i<sizeof(buffer2); ++i)
  {
    buffer2[i] = i;
  }

  ringbuffer.read_index = 100;
  ringbuffer.write_index = 100;

  if(!ringbuffer_add_data(&ringbuffer, buffer2, sizeof(buffer2)-1))
  {
    return -1;
  }
  read = ringbuffer_get_data(&ringbuffer, buffer3, sizeof(buffer3));

  ringbuffer.read_index = 100;
  ringbuffer.write_index = 100;

  read = ringbuffer_get_data(&ringbuffer, buffer3, sizeof(buffer3));

  // test write at the end with too much data

  ringbuffer.read_index = 2;
  ringbuffer.write_index = 127;

  if(ringbuffer_add_data(&ringbuffer, "Hello, World", strlen("Hello, World")))
  {
    return -1;
  }

  ringbuffer_init(&ringbuffer, buffer, sizeof(buffer));

  ringbuffer.read_index = 100;
  ringbuffer.write_index = 120;

  if (!ringbuffer_add_data(&ringbuffer, "12345678", 8))
  {
      return -1;
  }

  return 0;
}