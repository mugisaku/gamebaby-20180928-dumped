#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




image&
image::
assign(const image&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      auto  size = get_image_size();

      m_data = new uint8_t[size];

      std::memcpy(m_data,rhs.m_data,size);

      static_cast<image_header&>(*this) = static_cast<const image_header&>(rhs);
    }


  return *this;
}


image&
image::
assign(image&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_data,rhs.m_data);

      std::swap(static_cast<image_header&>(*this),
                static_cast<image_header&>(  rhs));
    }


  return *this;
}




void
image::
clear() noexcept
{
  delete[]  m_data          ;
            m_data = nullptr;

  static_cast<image_header&>(*this) = image_header(0,0);
}


bool
image::
read_png_from_file(const char*  path) noexcept
{
  chunk_list  ls;

  ls.read_png_from_file(path);

  chunk_set  set(ls);

  auto  ihdr = set.get_image_header();
  auto  idat = set.get_image_data();

  *this = idat.extract(ihdr);

  return true;
}




}




