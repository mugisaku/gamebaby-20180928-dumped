

set(SOURCES
  character.cpp
  character_data.cpp
  player.cpp
)


include_directories(./)
include_directories(../)
add_library("gbact" ${SOURCES})




