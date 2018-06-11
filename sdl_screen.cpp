#include"sdl.hpp"



namespace sdl{


namespace{


SDL_Window*      g_window;
SDL_Renderer*  g_renderer;
SDL_Texture*    g_texture;

int  g_width;
int  g_height;

SDL_Rect  g_dst_rect;


void
transfer(const gbstd::image&  src, uint8_t*  p_base, int  pitch) noexcept
{
  int  w = std::min(src.get_width() ,g_width );
  int  h = std::min(src.get_height(),g_height);

    for(int  y = 0;  y < h;  ++y)
    {
      auto  p = reinterpret_cast<uint16_t*>(p_base)        ;
                                            p_base += pitch;

        for(int  x = 0;  x < w;  ++x)
        {
          *p++ = src.get_const_pixel(x,y).color.code;
/*
          auto  c = src.get_const_pixel(x,y).color;

          *p++ = c.get_r255();
          *p++ = c.get_g255();
          *p++ = c.get_b255();
*/
        }
    }
}


}




void
update_screen(const gbstd::image&  img) noexcept
{
  int  pitch;

  void*  ptr;

    if(SDL_LockTexture(g_texture,nullptr,&ptr,&pitch) == 0)
    {
      transfer(img,reinterpret_cast<uint8_t*>(ptr),pitch);

      SDL_UnlockTexture(g_texture);

      SDL_RenderClear(g_renderer);
      SDL_RenderCopy(g_renderer,g_texture,nullptr,&g_dst_rect);

      SDL_RenderPresent(g_renderer);
    }
}


void
quit() noexcept
{
  SDL_DestroyTexture(g_texture);
  SDL_DestroyRenderer(g_renderer);
  SDL_DestroyWindow(g_window);

  SDL_Quit();

  std::_Exit(0);
}


void
init(int  w, int  h, double  scale) noexcept
{
  g_width  = w;
  g_height = h;

  g_dst_rect.x = 0;
  g_dst_rect.y = 0;
  g_dst_rect.w = w*scale;
  g_dst_rect.h = h*scale;


  SDL_Init(SDL_INIT_VIDEO);

  g_window = SDL_CreateWindow("GAME BABY - " __DATE__,0,0,g_dst_rect.w,g_dst_rect.h,0);

    if(!g_window)
    {
      printf("cannot create window\n");
    }


  g_renderer = SDL_CreateRenderer(g_window,-1,SDL_RENDERER_ACCELERATED);
  g_texture  = SDL_CreateTexture(g_renderer,SDL_PIXELFORMAT_ARGB1555,SDL_TEXTUREACCESS_STREAMING,w,h);
}


void
resize_screen(int  w, int  h) noexcept
{
    if(!g_window)
    {
      init(w,h);
    }

  else
    {
      SDL_SetWindowSize(g_window,w,h);

      SDL_DestroyTexture(g_texture);
                         g_texture  = SDL_CreateTexture(g_renderer,SDL_PIXELFORMAT_ARGB1555,SDL_TEXTUREACCESS_STREAMING,w,h);
    }
}


gbstd::image
make_screen_image() noexcept
{
  return gbstd::image(g_width,g_height);
}




}




