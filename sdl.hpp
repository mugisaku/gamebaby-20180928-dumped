#ifndef sdl_hpp_was_inclueded
#define sdl_hpp_was_inclueded


#include"libgbstd/image.hpp"
#include"libgbstd/controller.hpp"
#include<SDL.h>


namespace sdl{


void  update_screen(const gbstd::image&  img) noexcept;

void  save_screen(const char*  path) noexcept;

void  quit() noexcept;

void  init(int  w, int  h) noexcept;

void  update_controller() noexcept;


}




#endif




