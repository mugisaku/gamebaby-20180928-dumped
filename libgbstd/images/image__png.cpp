#include"libgbstd/image.hpp"
#include"libgbstd/string.hpp"
#include"libgbpng/png.hpp"



namespace gbstd{
namespace images{


using namespace gbpng;


void
image::
read_png_stream(const uint8_t*  ptr) noexcept
{
    try
    {
      direct_color_image  png;

      png.read_png_from_memory(ptr);

      resize(png.get_width(),png.get_height());

      auto  p = png.get_row_pointer(0);

        for(auto&  pix: m_pixels)
        {
          uint8_t  r = *p++;
          uint8_t  g = *p++;
          uint8_t  b = *p++;
          uint8_t  a = *p++;

          pix.color = a? color(r>>5,g>>5,b>>5):color();
        }
    }


    catch(std::exception&  e)
    {
      printf("error: %s\n",e.what());
    }
}


std::vector<uint8_t>
image::
make_png_stream() const noexcept
{
  std::vector<uint8_t>  buf;

    try
    {
      direct_color_image  png(get_width(),get_height());

      auto  p = png.get_row_pointer(0);

        for(auto  pix: m_pixels)
        {
          auto  color = pix.color;

            if(color)
            {
              *p++ = color.get_r255();
              *p++ = color.get_g255();
              *p++ = color.get_b255();
              *p++ = 255;
            }

          else
            {
              *p++ = 0;
              *p++ = 0;
              *p++ = 0;
              *p++ = 0;
            }
        }


      auto  ls = png.make_chunk_list(pixel_format::rgba,8);

      buf.resize(ls.calculate_stream_size());

      ls.write_png_to_memory(buf.data());
    }


    catch(std::exception&  e)
    {
      printf("error: %s\n",e.what());
    }


  return std::move(buf);
}


void
image::
load_png(const char*  path) noexcept
{
  auto  s = make_string_from_file(path);

  read_png_stream(reinterpret_cast<const uint8_t*>(s.data()));
}




}}




