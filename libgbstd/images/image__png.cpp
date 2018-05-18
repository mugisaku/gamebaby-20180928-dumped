#include"libgbstd/image.hpp"
#include"libgbstd/string.hpp"
#include<libpng/png.h>



namespace gbstd{
namespace images{




void
image::
load_from_png(const uint8_t*  data, size_t  data_size) noexcept
{
  png_image  png;

  png.version = PNG_IMAGE_VERSION;
  png.opaque = nullptr;

  png_image_begin_read_from_memory(&png,data,data_size);

  png.format = PNG_FORMAT_ARGB;

  int  stride = 4*png.width;

  auto  buffer = new uint8_t[stride*png.height];

  png_image_finish_read(&png,nullptr,buffer,stride,nullptr);

  resize(png.width,png.height);

  auto  p = buffer;

    for(auto&  pix: m_pixels)
    {
      uint8_t  a = *p++;
      uint8_t  r = *p++;
      uint8_t  g = *p++;
      uint8_t  b = *p++;

      pix.color = a? color(r>>5,g>>5,b>>5):color();
    }


  delete[] buffer;

  png_image_free(&png);
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
  png_image  png;

  std::memset(&png,0,sizeof(png));

  png.version = PNG_IMAGE_VERSION;
  png.opaque = nullptr;
  png.width = get_width();
  png.height = get_height();
  png.format = PNG_FORMAT_ARGB;


  int  stride = 4*png.width;

  auto  buffer = new uint8_t[stride*png.height];

  auto  p = buffer;

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


  png_image_write_to_file(&png,filepath,0,buffer,stride,nullptr);

  delete[] buffer;
}




}}




