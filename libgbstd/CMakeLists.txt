

set(SOURCES
  images/image.cpp
  images/image__load.cpp
  images/image__webp.cpp
  images/image__png.cpp
  images/image__draw.cpp
  images/image__transfer.cpp
  images/line_maker.cpp
  images/drawing_recorder.cpp
  texts/text.cpp
  texts/unicode.cpp
  texts/character_queue.cpp
  texts/string_form.cpp
  tasks/task_manager.cpp
  programs/value.cpp
  programs/context.cpp
  programs/program.cpp
  spaces/body.cpp
  spaces/object.cpp
  spaces/text_object.cpp
  spaces/space.cpp
  boards/area.cpp
  boards/board.cpp
  boards/board_view.cpp
  ios/riff_chunk.cpp
  ios/riff_subchunk_view.cpp
  ios/wave.cpp
  utility.cpp
  environment.cpp
)


include_directories(./)
include_directories(../)
add_library("gbstd" ${SOURCES})




