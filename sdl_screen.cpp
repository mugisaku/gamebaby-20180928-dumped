#include"sdl.hpp"



namespace sdl{


namespace{


SDL_Window*      window;
SDL_Renderer*  renderer;
SDL_Texture*    texture;


void
transfer(const gbstd::image&  src, uint8_t*  p_base, int  pitch) noexcept
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


}




void
update_screen(const gbstd::image&  img) noexcept
{
  int  pitch;

  void*  ptr;

    if(SDL_LockTexture(texture,nullptr,&ptr,&pitch) == 0)
    {
      transfer(img,reinterpret_cast<uint8_t*>(ptr),pitch);

      SDL_UnlockTexture(texture);

      SDL_RenderClear(renderer);
      SDL_RenderCopy(renderer,texture,nullptr,nullptr);

      SDL_RenderPresent(renderer);
    }
}


void
save_screen(const char*  path) noexcept
{
//  SDL_SaveBMP(surface,path);
}


void
quit() noexcept
{
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();
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


  renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
  texture  = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGB24,SDL_TEXTUREACCESS_STREAMING,w,h);
}


}




