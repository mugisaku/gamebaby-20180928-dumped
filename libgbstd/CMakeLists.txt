

set(SOURCES
  riff_chunk.cpp
  riff_subchunk_view.cpp
  wave.cpp
  images/image.cpp
  images/image_cursor.cpp
  images/image_frame.cpp
  images/image__png.cpp
  images/image__draw.cpp
  images/image__transfer.cpp
  images/line_maker.cpp
  playwork.cpp
  task.cpp
  group_task.cpp
  texts/text.cpp
  texts/unicode.cpp
  texts/character_queue.cpp
  texts/string_form.cpp
  widgets/widget.cpp
  widgets/container.cpp
  widgets/label.cpp
  widgets/button.cpp
  widgets/table.cpp
  widgets/icon_selector.cpp
  widgets/dial.cpp
  widgets/text_roll.cpp
  windows/window.cpp
  windows/window__draw_frame.cpp
  windows/window_manager.cpp
  utilities/utility.cpp
  utilities/random_number.cpp
  environment.cpp
  controller.cpp
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




