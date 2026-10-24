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


/**
 * @file example.c
 * 
 * @author Maximilian Schmid
 */


#include <stdio.h>
#include <string.h>

#include <ringbuffer/ringbuffer.h>

int main()
{
  // declare a buffer, which must stay in scope as long as the ringbuffer is used
  RINGBUFFER_DECLARE_MEMORY(buffer, 128);

  // the ringbuffer object
  ringbuffer_t ringbuffer;

  char read[128];
  size_t read_len = 0;

  // init the ringbuffer
  ringbuffer_init(&ringbuffer, buffer, sizeof(buffer));
  
  // add new data and check for success
  if(!ringbuffer_add_data(&ringbuffer, "Hello, Data", strlen("Hello, Data")))
  {
    printf("cold not insert data\n");
    return -1;
  }

  memset(read, 0, sizeof(read));

  // read the new data and print it
  read_len = ringbuffer_get_data(&ringbuffer, &read, sizeof(read));
  for(size_t i=0; i<read_len; ++i)
  {
    putc(read[i], stdout);
  }
  printf("\n");

  return 0;
}