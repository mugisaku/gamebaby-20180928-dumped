

set(SOURCES
  stream_context.cpp
  stream_reader.cpp
  stream_reader__skip.cpp
  stream_reader__read_number.cpp
)




include_directories(./)
include_directories(../)
add_library("tok" ${SOURCES})




