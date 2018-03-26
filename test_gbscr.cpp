#include"libgbscr/process.hpp"
#include"libgbscr/shared_string.hpp"


#ifdef EMSCRIPTEN
#include<emscripten.h>
#endif


using namespace gbscr;


namespace{


process
proc;


extern "C"{
#ifdef EMSCRIPTEN
EMSCRIPTEN_KEEPALIVE
#endif
int
fn(const char*  s)
{
  proc.clear();

  proc.load_from_string(s);

  proc.print();

  printf("\n");

  fflush(stdout);


  proc.call("main",{},nullptr);

  proc.run();

  fflush(stdout);


  return 0;
}
}


void
main_loop() noexcept
{
}


}




int
main(int  argc, char**  argv)
{
#ifdef EMSCRIPTEN
  emscripten_set_main_loop(main_loop,0,true);
#else
  auto  f = fopen(argv[1],"rb");

  gbstd::string  s;

    for(;;)
    {
      auto  c = fgetc(f);

        if(feof(f))
        {
          break;
        }


      s.append(c);
    }


  fn(s.data());
#endif

  return 0;
}




