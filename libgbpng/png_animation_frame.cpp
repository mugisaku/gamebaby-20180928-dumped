#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




animation_frame&
animation_frame::
assign(const frame_control&  fctl, const image&  img) noexcept
{
  m_control = fctl;

  m_image = img;

  return *this;
}


animation_frame&
animation_frame::
assign(const frame_control&  fctl, image&&  img) noexcept
{
  m_control = fctl;

  m_image = std::move(img);

  return *this;
}




}




