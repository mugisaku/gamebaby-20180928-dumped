

set(SOURCES
  character.cpp
  player.cpp
  hero.cpp
  enemy.cpp
  lady.cpp
  lady_monitor.cpp
  item.cpp
  meat.cpp
  bullet.cpp
  greeting_sphere.cpp
)


include_directories(./)
include_directories(../)
add_library("gbact" ${SOURCES})




