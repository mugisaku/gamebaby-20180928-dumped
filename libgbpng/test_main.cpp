#include"libgbpng/png.hpp"
#include<cstdio>
#include<vector>
#include<SDL.h>


using namespace gbpng;


SDL_Window*      g_window;
SDL_Renderer*  g_renderer;
SDL_Texture*    g_texture;


void
update_screen(const image&  img) noexcept
{
  int  pitch;

  void*  ptr;

    if(SDL_LockTexture(g_texture,nullptr,&ptr,&pitch) == 0)
    {
      auto  dst = reinterpret_cast<uint8_t*>(ptr);

        for(int  y = 0;  y < img.get_height();  ++y)
        {
          SDL_memcpy(dst,img.get_rgba_pointer(0,y),4*img.get_width());

          dst += pitch;
        }


      SDL_UnlockTexture(g_texture);

      SDL_RenderClear(g_renderer);
      SDL_RenderCopy(g_renderer,g_texture,nullptr,nullptr);

      SDL_RenderPresent(g_renderer);
    }
}




int
main(int  argc, char**  argv)
{
  chunk_list  chkls;

    if(chkls.read_png_from_file(argv[1]))
    {
//      chkls.print();
    }

  else
    {
      printf("read error\n");

      return 0;
    }


  movie  mov(chkls);

    if(!mov.get_width() || !mov.get_height())
    {
      return 0;
    }


//  chunk_list  tmp(img);

//  tmp.print();

//  write_png_to_file(tmp,"output.png");


  SDL_Init(SDL_INIT_VIDEO);

  g_window = SDL_CreateWindow("GAME BABY - " __DATE__,0,0,mov.get_width(),mov.get_height(),0);

    if(!g_window)
    {
      printf("cannot create window\n");
    }


  g_renderer = SDL_CreateRenderer(g_window,-1,SDL_RENDERER_ACCELERATED);
  g_texture  = SDL_CreateTexture(g_renderer,SDL_PIXELFORMAT_ABGR8888,SDL_TEXTUREACCESS_STREAMING,mov.get_width(),mov.get_height());

  int  i = 0;

  auto&  ls = mov.get_frame_list();

  static SDL_Event  evt;

    for(;;)
    {
      const image&  img = ls[i].get_image();

        while(SDL_PollEvent(&evt))
        {
            switch(evt.type)
            {
          case(SDL_WINDOWEVENT):
              update_screen(img);
              break;
          case(SDL_QUIT):
              goto QUIT;
              break;
            }
        }


      SDL_Delay(20);

      
    }


QUIT:
  SDL_DestroyTexture(g_texture);
  SDL_DestroyRenderer(g_renderer);
  SDL_DestroyWindow(g_window);

  SDL_Quit();


  return 0;
}




