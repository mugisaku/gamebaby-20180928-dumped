

set(SOURCES
  images/image.cpp
  images/image_cursor.cpp
  images/image__png.cpp
  images/image__draw.cpp
  images/image__transfer.cpp
  images/line_maker.cpp
  texts/text.cpp
  texts/unicode.cpp
  texts/character_queue.cpp
  texts/string_form.cpp
  widgets/widget.cpp
  widgets/container.cpp
  widgets/wrapper.cpp
  widgets/root.cpp
  widgets/label.cpp
  widgets/button.cpp
  widgets/table.cpp
  widgets/icon.cpp
  widgets/icon_selector.cpp
  widgets/dial.cpp
  widgets/text_roll.cpp
  widgets/menu.cpp
  widgets/radio_button.cpp
  widgets/check_button.cpp
  widgets/create_button_menu.cpp
  widgets/canvas.cpp
  windows/window.cpp
  windows/window__draw_frame.cpp
  windows/window_manager.cpp
  ios/binary_stream.cpp
  ios/binary_stream_reader.cpp
  ios/binary_stream_writer.cpp
  ios/riff_chunk.cpp
  ios/riff_subchunk_view.cpp
  ios/wave.cpp
  playwork.cpp
  task.cpp
  group_task.cpp
  utility.cpp
  garden.cpp
  environment.cpp
  controller.cpp
  menu_base.cpp
  roll_menu.cpp
  column_menu.cpp
)


include_directories(./)
include_directories(../)
add_library("gbstd" ${SOURCES})
target_link_libraries("gbstd")




