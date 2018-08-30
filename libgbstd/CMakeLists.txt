

set(SOURCES
  images/image.cpp
  images/image__load.cpp
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
  programs/process.cpp
  programs/program.cpp
  programs/common.cpp
  programs/sleep.cpp
  programs/indication.cpp
  programs/chooser.cpp
  spaces/object.cpp
  spaces/text_object.cpp
  ios/riff_chunk.cpp
  ios/riff_subchunk_view.cpp
  ios/wave.cpp
  ios/io.cpp
  utility.cpp
  environment.cpp
)


include_directories(./)
include_directories(../)
add_library("gbstd" ${SOURCES})




