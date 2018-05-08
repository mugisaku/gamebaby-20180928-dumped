#ifndef sdl_hpp_was_inclueded
#define sdl_hpp_was_inclueded


#include"libgbstd/image.hpp"
#include"libgbstd/control_device.hpp"
#include<SDL.h>


namespace sdl{


void  update_screen(const gbstd::image&  img) noexcept;

void  quit() noexcept;

void  init(int  w, int  h) noexcept;

void  resize_screen(int  w, int  h) noexcept;

gbstd::image  make_screen_image() noexcept;

gbstd::control_device&  update_control_device() noexcept;


}




#endif




