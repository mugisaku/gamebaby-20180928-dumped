#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




picture&
picture::
assign(const chunk_set&  set) noexcept
{

  return *this;
}


/*
chunk_list&
chunk_list::
assign(const picture&  pic) noexcept
{
  clear();

  put_chunk(pic.make_control_chunk());

  auto  inc_flag = pic.does_use_main_image_as_first_frame();

    if(inc_flag)
    {
      put_chunk(pic.make_frame_control_chunk());
    }


  auto  ihdr = pic.make_image_header();
  auto  idat = pic.make_image_data();

  put_chunk(ihdr.make_chunk());
  put_chunk(idat.make_chunk());

  uint32_t  seq_num = inc_flag? 1:0;

    for(auto&  frm: pic.get_frame_list())
    {
      put_chunk(frm.make_control_chunk(seq_num  ));
      put_chunk(frm.make_chunk(        seq_num++));
    }


  return *this;
}




*/


chunk
picture::
make_frame_control_chunk() const noexcept
{
  size_t  size = 4+4+4+4+4+2+2+1+1;

  uint8_t  buffer[size];

  uint8_t*  p = buffer;

  put_be32(0,p);
  put_be32(get_width(),p);
  put_be32(get_height(),p);
  put_be32(0,p);
  put_be32(0,p);
  put_be16(0,p);
  put_be16(0,p);

  *p++ = 0;
  *p++ = 0;

  return chunk(binary(buffer,size),"fcTL");
}


chunk
picture::
make_control_chunk() const noexcept
{
  size_t  size = 4+4;

  uint8_t  buffer[size];

  uint8_t*  p = buffer;

  put_be32(m_frame_list.size()+(m_use_main_image_as_first_frame? 1:0),p);
  put_be32(m_number_of_plays,p);

  return chunk(binary(buffer,size),"acTL");
}




}




