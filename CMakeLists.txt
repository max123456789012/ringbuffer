cmake_minimum_required(VERSION 3.20)

project(ringbuffer)

add_library(ringbuffer src/ringbuffer.c)
target_include_directories(ringbuffer PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include> 
)


option(ENABLE_TESTS "Enable Tests" ON)
option(ENABLE_EXAMPLES "Build Examples" ON)

if(ENABLE_TESTS)
  message("tests enabled")
  enable_testing()
  add_executable(ringbuffer_test tests/ringbuffer_test.c)
  target_link_libraries(ringbuffer_test ringbuffer)
  add_test(ringbuffer_test ringbuffer_test)
endif()

if(ENABLE_EXAMPLES)
  
  find_package(Threads REQUIRED)

  message("building examples")
  add_executable(example examples/example.c)
  target_link_libraries(example ringbuffer)

  add_executable(example_mt examples/example_mt.c)
  target_link_libraries(example_mt ringbuffer Threads::Threads)
endif()