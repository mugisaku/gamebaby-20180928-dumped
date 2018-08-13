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
set.print();
  m_frame_list.clear();

  m_index = 0;

    if(set.get_actl_chunk())
    {
      read_animation_data(set);
    }

  else
    {
      auto  ihdr = set.get_image_header();

      auto  idat = set.get_image_data();

      palette  plte;

      m_current = image(ihdr,&plte,idat);

      m_width  = m_current.get_width() ;
      m_height = m_current.get_height();
    }


  return *this;
}


void
movie::
read_animation_data(const chunk_set&  set) noexcept
{
  animation_control  actl(*set.get_actl_chunk());

  m_number_of_plays = actl.get_number_of_plays();

  auto  top_fctl_chunk = set.get_top_fctl_chunk();

  auto  ihdr = set.get_image_header();

  m_current  = image(ihdr.get_width(),ihdr.get_height());
  m_previous = image(ihdr.get_width(),ihdr.get_height());

  m_width  = m_current.get_width() ;
  m_height = m_current.get_height();


  palette  plte;

  frame_control  fctl;

    if(top_fctl_chunk)
    {
      fctl = frame_control(*top_fctl_chunk);


      auto  idat = set.get_image_data();

      image  img(ihdr,&plte,idat);

      m_frame_list.emplace_back(fctl,std::move(img));
    }


    for(auto&  e: set.get_animation_elements())
    {
      frame_data  fdat;

      e.get(fctl,fdat);


      image  img(ihdr,&plte,fctl,fdat);

      m_frame_list.emplace_back(fctl,std::move(img));
    }
}


void
movie::
render() noexcept
{
    if(m_frame_list.empty())
    {
      return;
    }


  auto&  frame = m_frame_list[m_index+1];

  auto&  fctl = frame.get_control();
  auto&   img = frame.get_image();

    switch(fctl.get_dispose_type())
    {
  case(dispose_type::none):
      image::copy(m_current,m_previous,0,0);
      break;
  case(dispose_type::background):
      image::copy(m_current,m_previous,0,0);
      m_current.fill();
      break;
  case(dispose_type::previous):
      image::copy(m_previous,m_current,0,0);
      break;
    }


    switch(fctl.get_blend_type())
    {
  case(blend_type::source):
      image::copy(img,m_current,fctl.get_x_offset(),fctl.get_y_offset());
      break;
  case(blend_type::over):
      image::blend(img,m_current,fctl.get_x_offset(),fctl.get_y_offset());
      break;
    }
}


void
movie::
advance() noexcept
{
  ++m_index;

    if(m_index >= m_frame_list.size())
    {
      m_index = 0;
    }
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




