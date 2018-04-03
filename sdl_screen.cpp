#include<SDL.h>
#include"libgbstd/image.hpp"
#include<cstdlib>
#ifdef EMSCRIPTEN
#include<emscripten.h>
#endif



namespace sdl{


namespace{


SDL_Window*      window;
SDL_Surface*    surface;
SDL_Renderer*  renderer;
SDL_Texture*    texture;

gbstd::palette
palette;


void
transfer1(const gbstd::image&  src, uint8_t*  p_base, int  pitch) noexcept
{
  int  w =  src.get_width();
  int  h = src.get_height();

    for(int  y = 0;  y < h;  ++y)
    {
      auto  p = p_base         ;
                p_base += pitch;

        for(int  x = 0;  x < w;  ++x)
        {
          *p++ = src.get_pixel_color(x,y,palette);
        }
    }
}


void
transfer2(const gbstd::image&  src, uint8_t*  p_base, int  pitch) noexcept
{
  int  w =  src.get_width();
  int  h = src.get_height();

    for(int  y = 0;  y < h;  ++y)
    {
      auto  p = p_base         ;
                p_base += pitch;

        for(int  x = 0;  x < w;  ++x)
        {
          *reinterpret_cast<uint16_t*>(p) = src.get_pixel_color(x,y,palette);

          p += sizeof(uint16_t);
        }
    }
}


void
transfer3(const gbstd::image&  src, uint8_t*  p_base, int  pitch) noexcept
{
  int  w =  src.get_width();
  int  h = src.get_height();

    for(int  y = 0;  y < h;  ++y)
    {
      auto  p = p_base         ;
                p_base += pitch;

        for(int  x = 0;  x < w;  ++x)
        {
          auto&  i = src.get_const_pixel(x,y).color_index;

          *p++ = i.get_r255();
          *p++ = i.get_g255();
          *p++ = i.get_b255();
        }
    }
}


void
transfer4(const gbstd::image&  src, uint8_t*  p_base, int  pitch) noexcept
{
  int  w =  src.get_width();
  int  h = src.get_height();

    for(int  y = 0;  y < h;  ++y)
    {
      auto  p = p_base         ;
                p_base += pitch;

        for(int  x = 0;  x < w;  ++x)
        {
          *reinterpret_cast<uint32_t*>(p) = src.get_pixel_color(x,y,palette);

          p += sizeof(uint32_t);
        }
    }
}


}




void
update_screen(const gbstd::image&  img) noexcept
{
  int  pitch;

    if(texture)
    {
      void*  ptr;

        if(SDL_LockTexture(texture,nullptr,&ptr,&pitch) == 0)
        {
          transfer3(img,reinterpret_cast<uint8_t*>(ptr),pitch);

          SDL_UnlockTexture(texture);

          SDL_RenderClear(renderer);
          SDL_RenderCopy(renderer,texture,nullptr,nullptr);

          SDL_RenderPresent(renderer);
        }
    }

  else
    {
      surface = SDL_GetWindowSurface(window);

      pitch = surface->pitch;

      int  bpp  = surface->format->BytesPerPixel;

        if(SDL_LockSurface(surface) == 0)
        {
          SDL_memset(surface->pixels,0,pitch*surface->h);

          auto  p_base = static_cast<uint8_t*>(surface->pixels);

            switch(bpp)
            {
          case(1): transfer1(img,p_base,pitch);break;
          case(2): transfer2(img,p_base,pitch);break;
          case(3): transfer3(img,p_base,pitch);break;
          case(4): transfer4(img,p_base,pitch);break;
          default:
              printf("unknown bpp %d\n",bpp);
            }


          SDL_UnlockSurface(surface);

          SDL_UpdateWindowSurface(window);
        }
    }
}


void
save_screen(const char*  path) noexcept
{
  SDL_SaveBMP(surface,path);
}


void
quit() noexcept
{
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  std::_Exit(0);
}


void
init(int  w, int  h) noexcept
{
  SDL_Init(SDL_INIT_VIDEO);

  window = SDL_CreateWindow("GAME BABY - " __DATE__,0,0,w,h,0);

    if(!window)
    {
      printf("cannot create window\n");
    }


  surface = SDL_GetWindowSurface(window);

    if(!surface)
    {
      printf("cannot get window surface\n");
    }


    for(int  r = 0;  r < 8;  ++r){
    for(int  g = 0;  g < 8;  ++g){
    for(int  b = 0;  b < 8;  ++b){
      gbstd::color_index  i(r,g,b);

      auto  color = SDL_MapRGB(surface->format,i.get_r255(),i.get_g255(),i.get_b255());

      palette.set_color(i,color);
    }}}


//  renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
//  texture  = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGB24,SDL_TEXTUREACCESS_STREAMING,w,h);
}


}




