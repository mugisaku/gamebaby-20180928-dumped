#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




animation_frame&
animation_frame::
assign(image&&  img, const frame_control&  fctl) noexcept
{
  m_image = std::move(img);

  m_sequence_number   = fctl.get_sequence_number();
  m_delay_numerator   = fctl.get_delay_numerator();
  m_delay_denominator = fctl.get_delay_denominator();


  return *this;
}




chunk
animation_frame::
make_control_chunk() const noexcept
{
  frame_control  fctl;

  fctl.set_sequence_number(m_sequence_number);
  fctl.set_width( m_image.get_width());
  fctl.set_height(m_image.get_height());
  fctl.set_delay_numerator(m_delay_numerator);
  fctl.set_delay_denominator(m_delay_denominator);
  fctl.set_dispose_type(dispose_type::background);

  return fctl.make_chunk();
}


chunk
animation_frame::
make_data_chunk() const noexcept
{
  auto  idat = m_image.make_image_data();

  frame_data  fdat(m_sequence_number,std::move(idat));

  return fdat.make_chunk();
}




}




