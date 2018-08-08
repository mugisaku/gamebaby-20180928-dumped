#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




void
picture::
seek_for_idat(chunk_list::iterator&  it, const chunk*&  actl, const chunk*&  fctl, const chunk*&  idat) noexcept
{
  actl = nullptr;
  fctl = nullptr;
  idat = nullptr;

    while(it)
    {
      auto&  chk = *it++;

        if(chk == "acTL")
        {
          actl = &chk;
        }

      else
        if(chk == "fcTL")
        {
          fctl = &chk;
        }

      else
        if(chk == "IDAT")
        {
          idat = &chk;

          break;
        }

      else
        if(chk == "IEND")
        {
          break;
        }
    }
}


picture&
picture::
assign(const chunk_list&  ls) noexcept
{
  m_frame_list.clear();

  auto  it = ls.begin();

    if(!it || (*it != "IHDR"))
    {
      printf("picture assign error: have no IHDR chunk\n");

      return *this;
    }


  const image_header  ihdr(*it++);

  const chunk*  actl;
  const chunk*  fctl;
  const chunk*  idat;

  seek_for_idat(it,actl,fctl,idat);

    if(!idat)
    {
      printf("picture assign error: have no IDAT chunk\n");

      return *this;
    }


  return *this;
}




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




