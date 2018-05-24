

set(SOURCES
  character.cpp
  player.cpp
)


include_directories(./)
include_directories(../)
add_library("gbact" ${SOURCES})




