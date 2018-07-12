

set(SOURCES
  square.cpp
  square_info.cpp
  board.cpp
  piece.cpp
  piece_info.cpp
  piece_object.cpp
)


include_directories(./)
include_directories(../)
add_library("gbwar" ${SOURCES})




