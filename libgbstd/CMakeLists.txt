

set(SOURCES
  images/image.cpp
  images/image__load.cpp
  images/image__webp.cpp
  images/image__png.cpp
  images/image__draw.cpp
  images/image__transfer.cpp
  images/line_maker.cpp
  images/drawing_recorder.cpp
  icons/icon.cpp
  texts/text.cpp
  texts/unicode.cpp
  texts/character_queue.cpp
  texts/string_form.cpp
  tasks/task_manager.cpp
  programs/value.cpp
  programs/context.cpp
  programs/program.cpp
  spaces/area.cpp
  spaces/body.cpp
  spaces/object.cpp
  spaces/space.cpp
  widgets/widget.cpp
  widgets/container.cpp
  widgets/node.cpp
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
  widgets/color_maker.cpp
  widgets/color_holder.cpp
  canvases/canvas.cpp
  canvases/canvas__update.cpp
  canvases/canvas__draw.cpp
  canvases/canvas__operate.cpp
  canvases/canvas__create_sub_widget.cpp
  windows/window.cpp
  windows/window__update_frame_image.cpp
  windows/window_manager.cpp
  ios/riff_chunk.cpp
  ios/riff_subchunk_view.cpp
  ios/wave.cpp
  utility.cpp
  environment.cpp
)


include_directories(./)
include_directories(../)
add_library("gbstd" ${SOURCES})




