cmake_minimum_required(VERSION 2.8)
project(gmbb)


find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "CCACHE_DIR=/tmp/ccachedir ccache")
endif(CCACHE_FOUND)


set(CMAKE_VERBOSE_MAKEFILE ON)


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()


set(CMAKE_C_COMPILER  "/usr/local/bin/gcc")
set(CMAKE_C_FLAGS   "-std=c1z -Werror")
set(CMAKE_CXX_COMPILER  "/usr/local/bin/g++")
set(CMAKE_CXX_FLAGS "-std=c++1z -Werror")
set(CMAKE_CXX_LINK_FLAGS  "-lstdc++ -lz -lpng -lSDL2 -lSDL2main")


set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS}   -O2 -march=core2 -fomit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2 -march=core2 -fomit-frame-pointer")
set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS}   -O0 -g")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS} -O0 -g")


if(EMSC)
  set(EXECUTABLE_EXTENSION ".html")
  set(CMAKE_CXX_COMPILER  em++)
  set(CMAKE_CXX_FLAGS "-Werror -std=c++1z -Wno-switch -O2")
  set(CMAKE_CXX_FLAGS_DEBUG "")
  set(CMAKE_CXX_LINK_FLAGS  "${CMAKE_CXX_LINK_FLAGS} -s USE_ZLIB=1 -s USE_LIBPNG=1 -s USE_SDL=2 --embed-file image.png --embed-file battle.gb.txt --embed-file large_font.gz --embed-file small_font.gz")
endif()




set(GAME_SOURCES
  Game/EventQueue.cpp
  Game/Shop.cpp
  Game/ShopManager.cpp
  Game/Actor.cpp
  Game/Director.cpp
  Game/EffectObject.cpp
  Game/Screen.cpp
  Game/Game.cpp
  Game/Game__Message.cpp
  Game/Game__MainMenu.cpp
  Game/Game__SackMenu.cpp
  Game/Game__ShopMenu.cpp
  Game/Game__Shopping.cpp
  Game/Game__Choosing.cpp
  Game/Game__StatusMonitor.cpp
  Game/Game__StatusReportor.cpp
  Game/Game__Debugger.cpp
  Game/Game__controll_hero_piece.cpp
  Game/Game__throw_item.cpp
  Game/Game__process_string.cpp
  Game/Game__process_script.cpp
  Game/Game__process_event.cpp
  Game/GameData.cpp
  Game/GameItem.cpp
  Game/GameItemSpecs.cpp
  Game/Shop.cpp
  Game/Sack.cpp
  Game/Hero.cpp
  Game/Role.cpp
  Game/Piece.cpp
  Game/Square.cpp
  Game/Board.cpp
  Game/Board__render.cpp
  Game/ScriptManager.cpp
  Game/Messembly.cpp
  Game/Messembly_Image.cpp
  Game/Messembly_ImageBuilder.cpp
)


add_subdirectory(libgbstd)
add_subdirectory(libgbsnd)
add_subdirectory(libgbdn)
add_subdirectory(libty)
add_subdirectory(libtok)
add_subdirectory(libipre)
add_subdirectory(Battle)

include_directories(./)
include_directories(/usr/include/SDL2)
#add_executable("testGBSTD${EXECUTABLE_EXTENSION}" testGBSTD.cpp)
add_executable("gbsnd_sdl${EXECUTABLE_EXTENSION}" gbsnd_sdl.cpp)
#add_executable("testBattle${EXECUTABLE_EXTENSION}" Battle/testMain.cpp)
#add_executable("testRandom${EXECUTABLE_EXTENSION}" Battle/testRandom.cpp)
#add_executable("testMessembly${EXECUTABLE_EXTENSION}" Game/testMessembly.cpp ${GAME_SOURCES})
#target_link_libraries("testGBSTD${EXECUTABLE_EXTENSION}" "gbstd")
target_link_libraries("gbsnd_sdl${EXECUTABLE_EXTENSION}" "gbsnd" "tok" "gbstd")
#target_link_libraries("testBattle${EXECUTABLE_EXTENSION}" "battle" "gbdn" "tok" "gbstd")


if(EMSC)
#add_custom_target(bincopy ALL COMMAND cp ../bin/image.png ../battle.gb.txt /usr/local/share/gmbb/large_font.gz /usr/local/share/gmbb/small_font.gz ./)
#add_dependencies("testBattle${EXECUTABLE_EXTENSION}" bincopy)
#set_target_properties("testBattle${EXECUTABLE_EXTENSION}"  PROPERTIES LINK_FLAGS "--shell-file ../shell.html")
endif()




