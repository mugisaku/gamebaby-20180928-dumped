#include"libgbstd/wave.hpp"
#include"libgbsnd/device.hpp"
#include"libgbsnd/script.hpp"
#include"libgbsnd/expr.hpp"
#include"libgbsnd/execution.hpp"


namespace{


gbsnd::script
script;


gbsnd::execution_context
ctx;


std::vector<uint8_t>
buffer;


void
output()
{
  constexpr int  len = 20;

  uint32_t  tmp_buf[len] = {0};

    for(auto&  sq: script.get_square_wave_list())
    {
      sq->output(tmp_buf,&tmp_buf[len]);
    }


    for(auto&  no: script.get_noise_list())
    {
      no->output(tmp_buf,&tmp_buf[len]);
    }


  auto  buf_end = tmp_buf+len;

    for(auto  src: tmp_buf)
    {
      auto  v = src>>16;

      v = (v > 0xFF)? 0xFF:v;

      buffer.emplace_back(v/8);
    }
}


void
show_usage() noexcept
{
  printf("使い方: gbsnd_nogui [オプション] ファイルパス\n");
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


  ctx.reset(script);

  ctx.call("main",{});

  uint32_t  time;

  constexpr int  number_of_samples_per_millisecond = gbsnd::number_of_samples_per_second/1000;

    for(;;)
    {
      ctx.run(gbsnd::millisecond{time/number_of_samples_per_millisecond});

        if(ctx.is_exited())
        {
          break;
        }


      output();

      time += 20;
    }


  gbstd::wave         wav;
  gbstd::wave_format  wavfmt;

  wavfmt.id = 1;
  wavfmt.sampling_rate = gbsnd::number_of_samples_per_second;	
  wavfmt.byte_rate =     gbsnd::number_of_samples_per_second;	
  wavfmt.number_of_channels = 1;	
  wavfmt.block_align = 1;	
  wavfmt.number_of_bits_per_sample = 8;


  wav.assign(buffer.data(),buffer.size(),wavfmt);

  auto  ff = fopen("__output.wav","wb");

    if(ff)
    {
      wav.save_to_file(ff);

      fclose(ff);
    }


  return 0;
}




