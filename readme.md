# Ringbuffer
A ring buffer implementation to let two async parts of a program communicate with each other.
There is only one provider of data and one consumer. The ring buffer uses a
provided buffer to operate on, so no dynamic allocations are required

## Adding to your Project:
### Requireremts to target patform

The project requires the following headers on the target platform:
- stdbool.h
- stddef.h
- stdint.h
- stdatomics.h

### Interrupt Service Routines
The ring buffer can be used to transmit data from an ISR to another part of the program,
but the atomic load and atomic store operations need to be lock free for that. Please check
that on your platform.

### Adding to CMAKE

Copy the repo in a folder and then use add_subdirectory

```cmake
add_subdirectory(libraries/ringbuffer)

...
add_executable(example)
target_link_libraries(example ringbuffer)
```

Or via fetch content:
```cmake

include(FetchContent)
FetchContent_Declare(
  ringbuffer
  GIT_REPOSITORY https://github.com/max123456789012/ringbuffer.git
  GIT_TAG        main
)
FetchContent_MakeAvailable(ringbuffer)

...
add_executable(example)
target_link_libraries(example ringbuffer)
```

### Adding to a C Program
Include the libraries header to use its functionality:
```c
#include <ringbuffer/ringbuffer.h>
```

Please checkout the example folder for inspiration.

## Documentation
Doxygen is used to generate documentation pages. To build the
documentation execute:
```bash
doxygen doc/Doxyfile
```
This will place the documentation in html format under doc/html