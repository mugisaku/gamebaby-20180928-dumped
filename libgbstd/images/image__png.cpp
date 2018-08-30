#include"libgbstd/image.hpp"
#include"libgbstd/string.hpp"
#include"libgbpng/png.hpp"



namespace gbstd{
namespace images{


using namespace gbpng;


void
image::
load_from_png(const uint8_t*  data, size_t  data_size) noexcept
{
    try
    {
      direct_color_image  png;

      png.read_png_from_memory(data);

      resize(png.get_width(),png.get_height());

      auto  p = png.get_row_pointer(0);

        for(auto&  pix: m_pixels)
        {
          uint8_t  a = *p++;
          uint8_t  r = *p++;
          uint8_t  g = *p++;
          uint8_t  b = *p++;

          pix.color = a? color(r>>5,g>>5,b>>5):color();
        }
    }


    catch(std::exception&  e)
    {
      printf("error: %s\n",e.what());
    }
}


void
image::
load_from_png(const char*  filepath) noexcept
{
  auto  s = make_string_from_file(filepath);

    if(s.size())
    {
      load_from_png(reinterpret_cast<const uint8_t*>(s.data()),s.size());
    }
}


void
image::
save_to_png(const char*  filepath) const noexcept
{
    try
    {
      direct_color_image  png(get_width(),get_height());

      auto  p = png.get_row_pointer(0);

        for(auto  pix: m_pixels)
        {
          auto  color = pix.color;

            if(color)
            {
              *p++ = 255;
              *p++ = color.get_r255();
              *p++ = color.get_g255();
              *p++ = color.get_b255();
            }

          else
            {
              *p++ = 0;
              *p++ = 0;
              *p++ = 0;
              *p++ = 0;
            }
        }


      png.write_png_to_file(filepath,pixel_format::rgba,8);
    }


    catch(std::exception&  e)
    {
      printf("error: %s\n",e.what());
    }
}




}}




