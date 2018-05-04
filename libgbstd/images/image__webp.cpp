#ifndef EMSCRIPTEN


#include"libgbstd/image.hpp"
#include"libgbstd/string.hpp"
#include<webp/encode.h>
#include<webp/decode.h>



namespace gbstd{
namespace images{




void
image::
load_from_webp(const char*  filepath) noexcept
{
  auto  s = make_string_from_file(filepath);

    if(s.size())
    {
      int  w;
      int  h;

      auto  decoded_data_ptr = WebPDecodeARGB(reinterpret_cast<const uint8_t*>(s.data()),s.size(),&w,&h);

        if(decoded_data_ptr)
        {
          resize(w,h);

          auto  p = decoded_data_ptr;

            for(auto&  pix: m_pixels)
            {
              uint8_t  a = *p++;
              uint8_t  r = *p++;
              uint8_t  g = *p++;
              uint8_t  b = *p++;

              pix.color = a? color(r>>5,g>>5,b>>5):color();
            }


          WebPFree(decoded_data_ptr);
        }
    }
}


void
image::
save_to_webp(const char*  filepath) const noexcept
{
  auto  f = fopen(filepath,"wb");

    if(f)
    {
      auto  w = get_width();
      auto  h = get_height();

      size_t  src_size = (4*w*h);

      auto  source_data_ptr = static_cast<uint8_t*>(malloc(src_size));

        if(source_data_ptr)
        {
          uint8_t*  p = source_data_ptr;

            for(auto  pix: m_pixels)
            {
                if(pix.color)
                {
                  *p++ = pix.color.get_r255();
                  *p++ = pix.color.get_g255();
                  *p++ = pix.color.get_b255();
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


          uint8_t*  encoded_data_ptr;

          auto  size = WebPEncodeLosslessRGBA(source_data_ptr,w,h,4*w,&encoded_data_ptr);

          p = encoded_data_ptr;

            while(size--)
            {
              fputc(*p++,f);
            }


          free(source_data_ptr);

          WebPFree(encoded_data_ptr);
        }


      fclose(f);
    }
}




}}




#endif




