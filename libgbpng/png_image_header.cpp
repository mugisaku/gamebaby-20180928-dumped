#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>
#include<png.h>




namespace gbpng{


image_header::
image_header(const chunk&  chk) noexcept
{
  binary_view  bv(chk);

  m_width     = bv.get_be32();
  m_height    = bv.get_be32();
  m_bit_depth = bv.get_8();
 
  set_pixel_format(static_cast<pixel_format>(bv.get_8()));

  auto  compression_method = bv.get_8();
  auto       filter_method = bv.get_8();

  m_interlaced = bv.get_8();
}




void
image_header::
set_pixel_format(pixel_format  fmt) noexcept
{
  m_pixel_format              = fmt;
  m_number_of_bytes_per_pixel = gbpng::get_number_of_bytes_per_pixel(fmt);
}


chunk
image_header::
make_chunk() const noexcept
{
  constexpr size_t  size = 13;

  binary  bin(size);

  binary_cursor  bc(bin);

  bc.put_be32(m_width );
  bc.put_be32(m_height);

  bc.put_8(m_bit_depth);
  bc.put_8(static_cast<int>(m_pixel_format));
  bc.put_8( 0);
  bc.put_8( 0);
  bc.put_8( 0);

  return chunk(std::move(bin),chunk_name("IHDR"));
}


void
image_header::
print() const noexcept
{
  printf("width: %d\n",m_width);
  printf("height: %d\n",m_height);
  printf("bit_depth: %d\n",m_bit_depth);
  printf("color_type: %s\n",get_pixel_format_name(m_pixel_format));


  printf("interlace: %s\n",m_interlaced? "true":"false");
}




}




