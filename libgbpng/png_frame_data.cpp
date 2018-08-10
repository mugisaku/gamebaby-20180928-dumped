#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




frame_data&
frame_data::
assign(uint32_t  seq_num, const std::vector<const chunk*>&  ls) noexcept
{
  m_sequence_number = seq_num;

  image_data::assign(ls);

  return *this;
}


frame_data&
frame_data::
assign(const chunk&  chk) noexcept
{
  const uint8_t*  p = chk.begin();

  m_sequence_number = get_be32(p);

  binary::assign(p,chk.get_data_size()-4);

  return *this;
}




chunk
frame_data::
make_chunk() const noexcept
{
  binary  bin(4+get_data_size());

  auto  p = bin.begin();

  put_be32(m_sequence_number,p);

  std::memcpy(p,begin(),get_data_size());

  return chunk(std::move(bin),chunk_name("fdAT"));
}




void
frame_data::
print() const noexcept
{
}


}




