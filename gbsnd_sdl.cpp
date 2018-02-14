#include"libgbsnd/device.hpp"
#include"libgbsnd/script.hpp"
#include"libgbsnd/expr.hpp"
#include"libgbsnd/execution.hpp"


#include<SDL.h>
#include<cstdlib>
#include<cstring>



namespace{


SDL_Window*
window;


SDL_AudioSpec
spec;


SDL_AudioDeviceID
id;


gbsnd::script
script;


gbsnd::execution_context
ctx;


constexpr int  number_of_samples = 1024;

uint32_t
buffer[number_of_samples];


void
callback(void*  userdata, uint8_t*  buf, int  len)
{
  memset(buffer,0,sizeof(uint32_t)*len);

    for(auto&  sq: script.get_square_wave_list())
    {
      sq->output(buffer,&buffer[len]);
    }


    for(auto&  no: script.get_noise_list())
    {
      no->output(buffer,&buffer[len]);
    }


  auto  buf_end = buf+len;

  const uint32_t*  src = buffer;

    while(buf < buf_end)
    {
      auto  v = (*src++)>>16;

      *buf++ = (v > 0xFF)? 0xFF:v;
    }
}


void
initialize()
{
  spec.freq     = gbsnd::number_of_samples_per_seconds;
  spec.format   = AUDIO_S8;
  spec.samples  = number_of_samples;
  spec.channels = 1;
  spec.callback = callback;

	 id = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(0,0),0,&spec,nullptr,0);

  SDL_PauseAudioDevice(id,0);
}


void
quit()
{
  SDL_CloseAudioDevice(id);

  SDL_DestroyWindow(window);

  SDL_Quit();

  fflush(stdout);

  std::_Exit(0);
}


void
fill_screen()
{
  SDL_FillRect(SDL_GetWindowSurface(window),nullptr,0);

  SDL_UpdateWindowSurface(window);
}


void
main_loop()
{
  static SDL_Event  evt;

    while(SDL_PollEvent(&evt))
    {
        switch(evt.type)
        {
      case(SDL_WINDOWEVENT):
             switch(evt.window.event)
             {
           case(SDL_WINDOWEVENT_EXPOSED):
               break;
             }
           break;
      case(SDL_QUIT):
           quit();
           break;
        }
    }


  auto  now = SDL_GetTicks();

  ctx.run(gbsnd::millisecond{now});
}


void
show_usage() noexcept
{
  printf("使い方: gbsnd_sdl [オプション] ファイルパス\n");
  printf("-asm        スクリプトの詳細を標準出力へ書き出す\n");
}


}


int
main(int  argc, char**  argv)
{
    if((argc == 1) || (argc >= 4))
    {
      show_usage();

      return 0;
    }


  const char*  file_path;

  bool  asm_output = false;

    if(argc == 2)
    {
      file_path = argv[1];
    }

  else
    if(argc == 3)
    {
        if(std::strcmp(argv[1],"-asm") == 0)
        {
          asm_output = true;
        }

      else
        {
          show_usage();

          return 0;
        }


      file_path = argv[2];
    }


  auto  f = fopen(file_path,"rb");

    if(!f)
    {
      printf("%sが開けません\n",file_path);

      return 0;
    }



  script = gbsnd::script::build_from_file(f,file_path);

    if(asm_output)
    {
      script.print();
    }


  constexpr int  w = 240;
  constexpr int  h = 240;

  SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);

  initialize();



  SDL_ShowCursor(SDL_DISABLE);

  window = SDL_CreateWindow("GAME BABY - " __DATE__,
0,0,
//                            SDL_WINDOWPOS_CENTERED,
//                            SDL_WINDOWPOS_CENTERED,
                            w,
                            h,0);

  fill_screen();

  ctx.reset(script);

  ctx.call("main",{});

    for(;;)
    {
      main_loop();

      SDL_Delay(20);
    }


  quit();

  return 0;
}




