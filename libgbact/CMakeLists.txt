

set(SOURCES
  routines/sleep_context.cpp
  routines/chooser_context.cpp
  routines/indication_context.cpp
  stage.cpp
  character.cpp
  player.cpp
  hero.cpp
  enemy.cpp
  lady.cpp
  lady_monitor.cpp
  item.cpp
  wall.cpp
  meat.cpp
  bullet.cpp
  greeting_sphere.cpp
)


include_directories(./)
include_directories(../)
add_library("gbact" ${SOURCES})




