#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




background_info&
background_info::
assign(const chunk&  chk) noexcept
{
  binary_view  bv(chk);

    switch(chk.get_data_size())
    {
  case(1):
      m_kind = kind::palette;
      m_data.i = bv.get_8();
      break;
  case(2):
      m_kind = kind::grayscale;

      m_data.l = bv.get_be16();
      break;
  case(6):
      m_kind = kind::color;

      m_data.r = bv.get_be16();
      m_data.g = bv.get_be16();
      m_data.b = bv.get_be16();
      break;
  default:
      printf("background_info assign error: unknown type\n");
    }


  return *this;
}




/*
chunk
image_data::
make_chunk() const noexcept
{
  return chunk(*this,chunk_name("IDAT"));
}




void
image_data::
print() const noexcept
{
}
*/


}




