#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




indexed_color_background_info&
indexed_color_background_info::
assign(const chunk&  chk) noexcept
{
  binary_view  bv(chk);

  m_value = bv.get_8();


  return *this;
}


chunk
indexed_color_background_info::
make_chunk() const noexcept
{
  binary  bin(1);

  binary_cursor  bc(bc);

  bc.put_8(m_value);

  return chunk(std::move(bin),chunk_name("bKGD"));
}


void
indexed_color_background_info::
print() const noexcept
{
  printf("");
}




grayscale_background_info&
grayscale_background_info::
assign(const chunk&  chk) noexcept
{
  binary_view  bv(chk);

  m_value = bv.get_be16();


  return *this;
}


chunk
grayscale_background_info::
make_chunk() const noexcept
{
  binary  bin(2);

  binary_cursor  bc(bc);

  bc.put_be16(m_value);

  return chunk(std::move(bin),chunk_name("bKGD"));
}


void
grayscale_background_info::
print() const noexcept
{
  printf("");
}




direct_color_background_info&
direct_color_background_info::
assign(const chunk&  chk) noexcept
{
  binary_view  bv(chk);

  m_r = bv.get_be16();
  m_g = bv.get_be16();
  m_b = bv.get_be16();


  return *this;
}


chunk
direct_color_background_info::
make_chunk() const noexcept
{
  binary  bin(6);

  binary_cursor  bc(bc);

  bc.put_be16(m_r);
  bc.put_be16(m_g);
  bc.put_be16(m_b);

  return chunk(std::move(bin),chunk_name("bKGD"));
}


void
direct_color_background_info::
print() const noexcept
{
  printf("");
}




}




