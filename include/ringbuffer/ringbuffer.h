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
 * @file ringbuffer.h
 * 
 * @author Maximilian Schmid
 * 
 * @brief ring buffer implementation to allow two async
 * running programs parts to exchange data.
 */

#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdatomic.h>

/**
 * @brief declare storeage for the ring buffer
 * 
 * @param name [in]: name of the ring buffer storeage
 * @param size [in]: useable capacity of the ring buffer. At least one
 * 
 * @details The ring buffer requires one additional byte to its useable capacity.
 * So one additional byte is acquired. 101 Bytes are reserved for a useable capacity of 100
 * for example.
 */
#define RINGBUFFER_DECLARE_MEMORY(name, size) \
  _Static_assert((size) >= 1, \
    "Ringbuffer buffersize must be at least 1 so the buffer must be at least of size 2"); \
  uint8_t name[size + 1];

typedef struct
{
  void *buffer;
  size_t buffer_size;
  atomic_size_t write_index;
  atomic_size_t read_index;
} ringbuffer_t;

/**
 * @brief init a ring buffer on the stack
 * 
 * @note ringbuffer must not go out of scope as long as this instance is used
 * @note buffer must also not go out of scope as long as the ring_buffer is used
 * @note The actual write capacity will be buffersize-1
 * 
 * @param ringbuffer [out]: pointer to initalized ring buffer
 * @param buffer [int]: memory to use for the ring buffer, which must not go out of scope
 *   as long as the ring buffer is used
 * @param buffersize [in]: buffer size in bytes
 * 
 * @returns true: on success, false: on failure
 */
bool ringbuffer_init(ringbuffer_t *ringbuffer, void *buffer, size_t buffersize);

/**
 * @brief check weather the ring buffer is empty
 * 
 * @param ringbuffer [in]: ring buffer to check emptiness
 * 
 * @returns true: ring buffer empty, false: ring buffer not empty
 */
bool ringbuffer_empty(const ringbuffer_t *ringbuffer);

/**
 * @brief get the available write capacity
 * 
 * @param ringbuffer [in]: ringbuffer to get capacity from
 * 
 * @returns number of writeable bytes
 */
size_t ringbuffer_capacity(const ringbuffer_t *ringbuffer);

/**
 * @brief obtain the amount of readable bytes
 * 
 * @param ringbuffer [in]: ring buffer to get available bytes from
 * 
 * @returns the amount of readable bytes
 */
size_t ringbuffer_available(const ringbuffer_t *ringbuffer);

/**
 * @brief write data in the ring buffer
 * 
 * @details adds data to the ring buffer. Ether the whole data with a length of size
 * is added or nothing
 * 
 * @param ringbuffer [in/out]: ring buffer to write data to
 * @param data [in]: data to add to the buffer
 * @param length [in]: size of the data to add in bytes
 * 
 * @returns true: the whole array was added, false: nothing was added
 */
bool ringbuffer_add_data(ringbuffer_t *ringbuffer, const void *data, size_t length);

/**
 * @brief read data from the ring buffer
 * 
 * @details The read data is stored in data. buffer size indicates teh maximum amount
 * of space in the destination buffer. 
 * 
 * @param ringbuffer [in/out]: ring buffer to read the data from
 * @param data [out]: pointer to buffer to store the read data
 * @param buffer_size [in]: available space in data in bytes
 * 
 * @returns the actual read size of the read data in bytes
 */
size_t ringbuffer_get_data(ringbuffer_t *ringbuffer, void *data, size_t buffer_size);


#endif // RINGBUFFER_H