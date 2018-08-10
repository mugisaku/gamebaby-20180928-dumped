cmake_minimum_required(VERSION 3.5)


find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "CCACHE_DIR=/tmp/ccachedir ccache")
endif(CCACHE_FOUND)


set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "-std=c++1z -Werror -I/usr/include/SDL2")


set(SOURCES
  png_binary.cpp
  png_binary__unfilter.cpp
  png_binary__filter.cpp
  png_picture.cpp
  png_animation_frame.cpp
  png_animation_control.cpp
  png_frame_control.cpp
  png_frame_data.cpp
  png_chunk_list.cpp
  png_chunk_set.cpp
  png_chunk.cpp
  png_chunk__calculate_crc.cpp
  png_palette.cpp
  png_image.cpp
  png_image_data.cpp
  png_image_header.cpp
)


include_directories(./)
include_directories(../)
add_library("gbpng" ${SOURCES})
add_executable(test_png test_main.cpp)
target_link_libraries(test_png "gbpng" "z" "SDL2" "SDL2main")




