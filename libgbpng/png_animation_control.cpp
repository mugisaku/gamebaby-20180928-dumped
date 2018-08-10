#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




animation_control&
animation_control::
assign(const chunk&  chk) noexcept
{
  const uint8_t*  p = chk.begin();

  m_number_of_frames = get_be32(p);
  m_number_of_plays  = get_be32(p);

  return *this;
}




chunk
animation_control::
make_chunk() const noexcept
{
  binary  bin(8);

  auto  p = bin.begin();

  put_be32(m_number_of_frames,p);
  put_be32(m_number_of_plays ,p);

  return chunk(std::move(bin),chunk_name("acTL"));
}




void
animation_control::
print() const noexcept
{
}


}




