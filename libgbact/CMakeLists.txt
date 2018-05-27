

set(SOURCES
  character.cpp
  player.cpp
  hero.cpp
  enemy.cpp
  bullet.cpp
  greeting_sphere.cpp
)


include_directories(./)
include_directories(../)
add_library("gbact" ${SOURCES})




