

set(SOURCES
  icons/icon.cpp
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
  canvases/canvas__make_apng_stream.cpp
  canvases/canvas__operate.cpp
  canvases/canvas__create_sub_widget.cpp
  windows/window.cpp
  windows/window__update_frame_image.cpp
  windows/window_manager.cpp
  presets/create_graphics_editor.cpp
)


include_directories(./)
include_directories(../)
add_library("gbgui" SHARED ${SOURCES})




