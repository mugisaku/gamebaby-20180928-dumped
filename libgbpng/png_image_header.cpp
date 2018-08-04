#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>
#include<png.h>




namespace gbpng{


image_header::
image_header(const chunk&  chk) noexcept
{
  auto  src = chk.get_data();

  m_width     = get_be32(src);
  m_height    = get_be32(src);
  m_bit_depth = *src++;
  m_flags     = *src++;

  auto  compression_method = *src++;
  auto       filter_method = *src++;

  m_interlaced = *src++;
}




int
image_header::
get_number_of_bytes_per_pixel() const noexcept
{
    if(does_use_palette())
    {
      return 1;
    }

  else
    {
        if(does_use_color())
        {        
          return does_use_alpha()? 4:3;
        }

      else
        if(does_use_alpha())
        {        
          return 2;
        }

      else
        {        
          return 1;
        }
    }


  return 1;
}


chunk
image_header::
make_chunk() const noexcept
{
  constexpr size_t  size = 13;

  uint8_t  buffer[size];

  auto  p = buffer;

  put_be32(m_width ,p);
  put_be32(m_height,p);

  *p++ = m_bit_depth;
  *p++ = m_flags;
  *p++ = 0;
  *p++ = 0;
  *p++ = 0;

  return chunk(size,chunk_name("IHDR"),buffer);
}


void
image_header::
print() const noexcept
{
  printf("width: %d\n",m_width);
  printf("height: %d\n",m_height);
  printf("bit_depth: %d\n",m_bit_depth);
  printf("color_type: %s %s %s\n",
    does_use_color()? "color ":"",
    does_use_palette()? "palette ":"",
    does_use_alpha()? "alpha ":""
  );


  printf("interlace: %s\n",m_interlaced? "true":"false");
}




}




