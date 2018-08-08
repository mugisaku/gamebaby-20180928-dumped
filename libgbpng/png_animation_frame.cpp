#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




chunk
animation_frame::
make_chunk(uint32_t  sequence_number) const noexcept
{
  auto  idat = image::make_image_data();

  size_t  size = 4+idat.get_data_size();

  uint8_t  buffer[size];

  uint8_t*  p = buffer;

  put_be32(sequence_number,p);

  std::memcpy(p,idat.get_data(),idat.get_data_size());

  return chunk(binary(buffer,size),"fdAT");
}




chunk
animation_frame::
make_control_chunk(uint32_t  sequence_number) const noexcept
{
  size_t  size = 4+4+4+4+4+2+2+1+1;

  uint8_t  buffer[size];

  uint8_t*  p = buffer;

  put_be32(sequence_number,p);
  put_be32(get_width(),p);
  put_be32(get_height(),p);
  put_be32(m_x_offset,p);
  put_be32(m_y_offset,p);
  put_be16(m_delay_numerator,p);
  put_be16(m_delay_denominator,p);

  *p++ = static_cast<int>(m_dispose_type);
  *p++ = static_cast<int>(m_blend_type);

  return chunk(binary(buffer,size),"fcTL");
}




}




