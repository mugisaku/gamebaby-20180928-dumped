#include"libgbstd/image.hpp"
#include<cstring>
#include<libpng/png.h>



namespace gbstd{
namespace images{




namespace{
void
read(png_structp  png, png_bytep  buf, png_size_t  size)
{
  auto&  r = *static_cast<binary_stream_reader*>(png_get_io_ptr(png));

    while(size--)
    {
      *buf++ = r.get();
    }
}
}


void
image::
load_png(binary_stream_reader&  r) noexcept
{
  auto  png      = png_create_read_struct(PNG_LIBPNG_VER_STRING,nullptr,nullptr,nullptr);
  auto  png_info = png_create_info_struct(png);

  png_set_read_fn(png,&r,read);

  png_read_info(png,png_info);


  const auto      w = png_get_image_width( png,png_info);
  const auto      h = png_get_image_height(png,png_info);
  const auto  depth = png_get_bit_depth(   png,png_info);

    if(depth >= 16)
    {
      png_set_strip_16(png);
    }


  resize(w,h);

  const auto  color_type = png_get_color_type(png,png_info);

  const bool  a_valid = (color_type&PNG_COLOR_MASK_ALPHA);

    if((color_type == PNG_COLOR_TYPE_GRAY) && (depth < 8))
    {
//      png_set_gray_1_2_4_to_8(png);
    }


    if((color_type == PNG_COLOR_TYPE_GRAY      ) ||
       (color_type == PNG_COLOR_TYPE_GRAY_ALPHA))
    {
      png_set_gray_to_rgb(png);
    }


    if(color_type == PNG_COLOR_TYPE_PALETTE)
    {
      png_set_palette_to_rgb(png);
    }


    if(color_type == PNG_COLOR_TYPE_RGB_ALPHA)
    {
    }


    if(color_type == PNG_COLOR_TYPE_RGB)
    {
//
    }


    if(depth < 8)
    {
      png_set_packing(png);
    }


  auto  buffer = new uint8_t[w*4];

  auto  dst = m_pixels.begin();

  fill();

    for(int  y = 0;  y < h;  ++y)
    {
      png_read_row(png,buffer,nullptr);

      const uint8_t*  src = buffer;

        for(int  x = 0;  x < w;  ++x)
        {
          auto&  color = *dst++;

          color = predefined::null;

          uint8_t  r = *src++;
          uint8_t  g = *src++;
          uint8_t  b = *src++;
          uint8_t  a = 1;

          uint8_t  first_r;
          uint8_t  first_g;
          uint8_t  first_b;

            if(a_valid)
            {
              a = *src++;
            }


            if(!x && !y)
            {
              first_r = r;
              first_g = g;
              first_b = b;
            }


            if(a && ((r != first_r) || (g != first_g) || (b != first_b)))
            {
              color = color_index(r>>5,g>>5,b>>5);
            }
        }
    }


  delete[]  buffer;

  png_read_end(png,png_info);
  png_destroy_read_struct(&png,&png_info,nullptr);
}


}}





