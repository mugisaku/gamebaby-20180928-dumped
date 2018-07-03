#include"libgbstd/io.hpp"




namespace gbstd{


void
save_file(gbstd::string_view  filename, gbstd::string_view  content) noexcept
{
  auto  f = fopen(filename.data(),"wb");

    if(f)
    {
        for(auto  c: content)
        {
          fputc(c,f);
        }


      fclose(f);
    }
}




}




