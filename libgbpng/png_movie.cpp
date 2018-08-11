#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




movie&
movie::
assign(const chunk_list&  ls) noexcept
{
  chunk_set  set(ls);

  return assign(set);
}


movie&
movie::
assign(const chunk_set&  set) noexcept
{
  m_frame_list.clear();

  auto  ihdr = set.get_image_header();

  m_width  = ihdr.get_width() ;
  m_height = ihdr.get_height();


  auto  actl_chunk = set.get_actl_chunk();

    if(actl_chunk)
    {
      animation_control  actl(*actl_chunk);

      m_number_of_plays = actl.get_number_of_plays();

      auto  top_fctl_chunk = set.get_top_fctl_chunk();

      palette  plte;

        if(top_fctl_chunk)
        {
          auto  idat = set.get_image_data();

          image  img(ihdr,&plte,idat);

          m_frame_list.emplace_back(std::move(img),frame_control());
        }
    }

  else
    {
      auto  idat = set.get_image_data();

      palette  plte;

      image  img(ihdr,&plte,idat);

      m_frame_list.emplace_back(std::move(img),frame_control());
    }


  return *this;
}


chunk
movie::
make_control_chunk() const noexcept
{
  size_t  size = 4+4;

  uint8_t  buffer[size];

  uint8_t*  p = buffer;

  put_be32(m_frame_list.size(),p);
  put_be32(m_number_of_plays,p);

  return chunk(binary(buffer,size),"acTL");
}




}




