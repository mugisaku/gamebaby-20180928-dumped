

set(SOURCES
  images/image.cpp
  images/image_cursor.cpp
  images/image__png.cpp
  images/image__draw.cpp
  images/image__transfer.cpp
  images/line_maker.cpp
  images/icon.cpp
  images/drawing_recorder.cpp
  texts/text.cpp
  texts/unicode.cpp
  texts/character_queue.cpp
  texts/string_form.cpp
  widgets/widget.cpp
  widgets/container.cpp
  widgets/wrapper.cpp
  widgets/frame.cpp
  widgets/root.cpp
  widgets/label.cpp
  widgets/button.cpp
  widgets/table.cpp
  widgets/icon_selector.cpp
  widgets/dial.cpp
  widgets/text_roll.cpp
  widgets/menu.cpp
  widgets/radio_button.cpp
  widgets/check_button.cpp
  widgets/create_button_menu.cpp
  widgets/canvas.cpp
  widgets/canvas__draw.cpp
  widgets/canvas__operate.cpp
  widgets/canvas__create_sub_widget.cpp
  widgets/color_maker.cpp
  windows/window.cpp
  windows/window__update_frame_image.cpp
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




