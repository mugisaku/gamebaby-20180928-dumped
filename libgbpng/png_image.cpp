#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




image&
image::
operator=(const image&  rhs) noexcept
{
    if(this != &rhs)
    {
      assign(rhs.get_width(),rhs.get_height(),rhs.get_pixel(0,0));
    }


  return *this;
}


image&
image::
operator=(image&&  rhs) noexcept
{
    if(this != &rhs)
    {
      delete[] m_rgba_buffer          ;
               m_rgba_buffer = nullptr;

      std::swap(m_width ,rhs.m_width );
      std::swap(m_height,rhs.m_height);
      std::swap(m_rgba_buffer,rhs.m_rgba_buffer);
    }


  return *this;
}




image&
image::
assign(int  w, int  h, uint8_t*  data) noexcept
{
    if(data)
    {
      delete[] m_rgba_buffer       ;
               m_rgba_buffer = data;


      m_width  = w;
      m_height = h;
    }

  else
    {
      resize(w,h);
    }


  return *this;
}


image&
image::
assign(int  w, int  h, const uint8_t*  src_data) noexcept
{
  resize(w,h);

    if(src_data)
    {
      std::memcpy(m_rgba_buffer,src_data,4*w*h);
    }


  return *this;
}




void
image::
clear() noexcept
{
  std::memset(m_rgba_buffer,0,4*m_width*m_height);
}


void
image::
resize(int  w, int  h) noexcept
{
  const size_t  size = 4*w*h;

  delete[] m_rgba_buffer                    ;
           m_rgba_buffer = new uint8_t[size];

  m_width  = w;
  m_height = h;

  std::memset(m_rgba_buffer,0,size);
}


}




