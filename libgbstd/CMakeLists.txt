

set(SOURCES
  image.cpp
  image__png.cpp
  image__draw.cpp
  image__transfer.cpp
  image__print.cpp
  font.cpp
  playwork.cpp
  task.cpp
  group_task.cpp
  text.cpp
  text_buffer.cpp
  text_roll.cpp
  text_roll_iterator.cpp
  unicode.cpp
  environment.cpp
  random_number.cpp
  controller.cpp
  line_maker.cpp
  window.cpp
  window__draw_frame.cpp
  menu_base.cpp
  roll_menu.cpp
  column_menu.cpp
  binary_stream.cpp
  binary_stream_reader.cpp
  binary_stream_writer.cpp
)


include_directories(./)
include_directories(../)
add_library("gbstd" ${SOURCES})
target_link_libraries("gbstd")




