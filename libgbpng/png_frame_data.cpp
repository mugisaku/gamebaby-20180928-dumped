#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




frame_data&
frame_data::
assign(uint32_t  seq_num, const std::vector<const chunk*>&  ls) noexcept
{
  m_sequence_number = seq_num;

  size_t  size = 0;

    for(auto  chk: ls)
    {
      size += chk->get_data_size()-4;
    }


  resize(size);

  auto  p = begin();

    for(auto  chk: ls)
    {
      const uint8_t*  it = chk->begin();

      auto  n = get_be32(it);

        if(n != m_sequence_number)
        {
          printf("frame_data assign error: wrong sequence number\n");

          break;
        }


      auto  current_size = chk->get_data_size()-4;

      std::memcpy(p,it,current_size);

      p += current_size;
    }


  return *this;
}


frame_data&
frame_data::
assign(uint32_t  seq_num, image_data&&  idat) noexcept
{
  m_sequence_number = seq_num;

  binary::assign(std::move(idat));

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




