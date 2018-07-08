

set(SOURCES
  processes/edit.cpp
  processes/play.cpp
  stage.cpp
  character.cpp
  player.cpp
  hero.cpp
  enemy.cpp
  lady.cpp
  lady_monitor.cpp
  item.cpp
  boy.cpp
  wall.cpp
  meat.cpp
  bullet.cpp
  greeting_sphere.cpp
)


include_directories(./)
include_directories(../)
add_library("gbact" ${SOURCES})




