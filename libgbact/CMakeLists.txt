

set(SOURCES
  character.cpp
  character_data.cpp
  player.cpp
  hero.cpp
  enemy.cpp
  bullet.cpp
)


include_directories(./)
include_directories(../)
add_library("gbact" ${SOURCES})




