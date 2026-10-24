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
#include <threads.h>

#include <time.h>

#ifdef __WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <ringbuffer/ringbuffer.h>

int thrd1();
int thrd2();

RINGBUFFER_DECLARE_MEMORY(buffer, 128)

ringbuffer_t ringbuffer;

const char abc[] = "abcdefghijklmnopqrstuvwxyz";

int main()
{

  thrd_t thread_1;
  thrd_t thread_2;

  ringbuffer_init(&ringbuffer, buffer, sizeof(buffer));

  if(thrd_success != thrd_create(&thread_1, thrd1, NULL))
  {
    printf("could not create thread 1\n");
  }
  if(thrd_success != thrd_create(&thread_2, thrd2, NULL))
  {
    printf("could not create thread 2\n");
  }

  if(thrd_success != thrd_join(thread_1, NULL) ||
    thrd_success != thrd_join(thread_2, NULL))
  {
    printf("join failed\n");
  }

  sleep(1);
  return 0;
}

int thrd1()
{
  for(size_t i=0; i<10000; ++i)
  {
    if(!ringbuffer_add_data(&ringbuffer, abc, sizeof(abc)))
    {
      // handle insertion error
      // printf("could not insert\n");
    }
  }
  return 0;
}

int thrd2()
{
  uint8_t buffer[128];
  size_t read = 0;

  size_t successful_read = 0;

  struct timespec delay = {
    .tv_sec = 0,
    .tv_nsec = 1000000L  // 1 ms = 1,000,000 ns
  };

  for(unsigned int i=0; i<10000; ++i)
  {
    read = ringbuffer_get_data(&ringbuffer, buffer, sizeof(buffer));
    if(read)
    {
      for(int j=0; j<read; ++j)
      {
        putc(buffer[j], stdout);
      }
      printf("\n");
    }
    thrd_sleep(&delay, NULL);
  }

  return 0;
}