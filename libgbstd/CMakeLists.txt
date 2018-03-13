

set(SOURCES
  riff_chunk.cpp
  riff_subchunk_view.cpp
  wave.cpp
  images/image.cpp
  images/image_frame.cpp
  images/image__png.cpp
  images/image__draw.cpp
  images/image__transfer.cpp
  playwork.cpp
  task.cpp
  group_task.cpp
  texts/text.cpp
  texts/text_buffer.cpp
  texts/text_roll.cpp
  texts/text_roll_iterator.cpp
  widget.cpp
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
  binary_streams/binary_stream.cpp
  binary_streams/binary_stream_reader.cpp
  binary_streams/binary_stream_writer.cpp
)


include_directories(./)
include_directories(../)
add_library("gbstd" ${SOURCES})
target_link_libraries("gbstd")




