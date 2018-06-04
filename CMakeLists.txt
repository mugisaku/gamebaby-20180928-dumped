cmake_minimum_required(VERSION 2.8)
project(gmbb)


if(SLEEP)
  set(SEEP_CMD "sleep 1;")
endif()


find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${SLEEP_CMD} CCACHE_DIR=/tmp/ccachedir ccache")
endif(CCACHE_FOUND)


set(CMAKE_VERBOSE_MAKEFILE ON)


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()


set(CMAKE_C_COMPILER  "/usr/local/bin/gcc")
set(CMAKE_C_FLAGS   "-std=c1z -Werror")
set(CMAKE_CXX_COMPILER  "/usr/local/bin/g++")
set(CMAKE_CXX_FLAGS "-std=c++1z -Werror")
set(CMAKE_CXX_LINK_FLAGS  "-lstdc++ -lSDL2 -lSDL2main")


set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS}   -O2 -march=core2 -fomit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2 -march=core2 -fomit-frame-pointer")
set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS}   -O0 -g")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS} -O0 -g")


if(EMSC)
  set(EXECUTABLE_EXTENSION ".html")
  set(CMAKE_CXX_COMPILER  em++)
  set(CMAKE_CXX_FLAGS "-Werror -std=c++1z -Wno-switch -Os")
  set(CMAKE_CXX_FLAGS_DEBUG "")
  set(EMSC_PORTS "-s USE_SDL=2 -s USE_LIBPNG=1")
  set(CMAKE_CXX_LINK_FLAGS  "${CMAKE_CXX_LINK_FLAGS} --shell-file ../shell.html --preload-file __resources/character.png --preload-file __resources/bg.png ${EMSC_PORTS}")
endif()




set(SDL_SOURCES
  sdl_screen.cpp
  sdl_controller.cpp
)


add_subdirectory(libgbstd)
add_subdirectory(libgbact)
#add_subdirectory(libgbsnd)
#add_subdirectory(libgbscr)

include_directories(./)
include_directories(/usr/include/SDL2)
#add_executable("mkfont_sdl${EXECUTABLE_EXTENSION}" mkfont_sdl.cpp ${SDL_SOURCES})
#add_executable("mkicon_sdl${EXECUTABLE_EXTENSION}" mkicon_sdl.cpp ${SDL_SOURCES})
#add_executable("mkforest_sdl${EXECUTABLE_EXTENSION}" mkforest_sdl.cpp ${SDL_SOURCES})
add_executable("mkanigra_sdl${EXECUTABLE_EXTENSION}" mkanigra_sdl.cpp ${SDL_SOURCES})
add_executable("game_sdl${EXECUTABLE_EXTENSION}" game_sdl.cpp ${SDL_SOURCES})
#add_executable("winmantester${EXECUTABLE_EXTENSION}" winmantester_sdl.cpp ${SDL_SOURCES})
#add_executable("growin${EXECUTABLE_EXTENSION}" growin_sdl.cpp)
#add_executable("test_gbscr${EXECUTABLE_EXTENSION}" test_gbscr.cpp)
#add_executable("testGBSTD${EXECUTABLE_EXTENSION}" testGBSTD.cpp)
#add_executable("gbsnd_test${EXECUTABLE_EXTENSION}" gbsnd_test.cpp)
#add_executable("gbsnd_sdl${EXECUTABLE_EXTENSION}" gbsnd_sdl.cpp)
#add_executable("gbsnd_nogui${EXECUTABLE_EXTENSION}" gbsnd_nogui.cpp)
#add_executable("landriser${EXECUTABLE_EXTENSION}" landriser_sdl.cpp)
#add_executable("piecefitter${EXECUTABLE_EXTENSION}" piecefitter_sdl.cpp piece.cpp)
#add_executable("hos_vm${EXECUTABLE_EXTENSION}" hos_vm.cpp)
#add_executable("stk_vm${EXECUTABLE_EXTENSION}" stk_vm.cpp)
#add_executable("reg_vm${EXECUTABLE_EXTENSION}" reg_vm.cpp)
#add_executable("testBattle${EXECUTABLE_EXTENSION}" Battle/testMain.cpp)
#add_executable("testRandom${EXECUTABLE_EXTENSION}" Battle/testRandom.cpp)
#add_executable("testMessembly${EXECUTABLE_EXTENSION}" Game/testMessembly.cpp ${GAME_SOURCES})
#target_link_libraries("test_gbscr${EXECUTABLE_EXTENSION}" "gbscr" "gbstd")
#target_link_libraries("mkfont_sdl${EXECUTABLE_EXTENSION}" "gbstd")
#target_link_libraries("mkicon_sdl${EXECUTABLE_EXTENSION}" "gbstd")
#target_link_libraries("mkforest_sdl${EXECUTABLE_EXTENSION}" "gbstd")
target_link_libraries("mkanigra_sdl${EXECUTABLE_EXTENSION}" "gbstd" "png" "z" "webp" "pthread")
#target_link_libraries("markit_sdl${EXECUTABLE_EXTENSION}" "gbstd" "markit")
#target_link_libraries("winmantester${EXECUTABLE_EXTENSION}" "gbstd")
#target_link_libraries("growin${EXECUTABLE_EXTENSION}" "gbstd")
target_link_libraries("game_sdl${EXECUTABLE_EXTENSION}" "gbact" "gbstd" "png" "z")
#target_link_libraries("landriser${EXECUTABLE_EXTENSION}" "gbstd")
#target_link_libraries("gbsnd_test${EXECUTABLE_EXTENSION}" "gbsnd" "tok" "gbstd")
#target_link_libraries("gbsnd_sdl${EXECUTABLE_EXTENSION}" "gbsnd" "tok" "gbstd")
#target_link_libraries("gbsnd_nogui${EXECUTABLE_EXTENSION}" "gbsnd" "tok" "gbstd")
#target_link_libraries("testBattle${EXECUTABLE_EXTENSION}" "battle" "gbdn" "tok" "gbstd")


if(EMSC)
endif()




