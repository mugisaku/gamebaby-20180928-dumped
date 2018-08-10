#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




frame_control&
frame_control::
assign(const chunk&  chk) noexcept
{
  const uint8_t*  p = chk.begin();

  m_sequence_number   = get_be32(p);
  m_width             = get_be32(p);
  m_height            = get_be32(p);
  m_x_offset          = get_be32(p);
  m_y_offset          = get_be32(p);
  m_delay_numerator   = get_be16(p);
  m_delay_denominator = get_be16(p);

  m_dispose_type = static_cast<dispose_type>(*p++);
  m_blend_type   = static_cast<  blend_type>(*p++);

  return *this;
}


chunk
frame_control::
make_chunk() const noexcept
{
  binary  bin(26);

  auto  p = bin.begin();

  put_be32(m_sequence_number,p);
  put_be32(m_width,p);
  put_be32(m_height,p);
  put_be32(m_x_offset,p);
  put_be32(m_y_offset,p);
  put_be16(m_delay_numerator,p);
  put_be16(m_delay_denominator,p);

  *p++ = static_cast<int>(m_dispose_type);
  *p++ = static_cast<int>(m_blend_type);

  return chunk(std::move(bin),chunk_name("fcTL"));
}




void
frame_control::
print() const noexcept
{
}


}




