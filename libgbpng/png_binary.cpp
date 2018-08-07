#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




binary&
binary::
assign(const binary&  rhs) noexcept
{
    if(this != &rhs)
    {
      assign(rhs.m_data,rhs.m_data_size);
    }


  return *this;
}


binary&
binary::
assign(binary&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear_data();

      std::swap(m_data     ,rhs.m_data     );
      std::swap(m_data_size,rhs.m_data_size);
    }


  return *this;
}




/*
binary&
binary::
assign(void*  ptr, uint32_t  size) noexcept
{
  m_data_size = size;

  delete[] m_data                                  ;
           m_data = reinterpret_cast<uint8_t*>(ptr);


  return *this;
}
*/


binary&
binary::
assign(const void*  ptr, uint32_t  size) noexcept
{
  m_data_size = size;

  delete[] m_data                           ;
           m_data = new uint8_t[m_data_size];

  std::memcpy(m_data,ptr,m_data_size);


  return *this;
}


void
binary::
clear_data() noexcept
{
  m_data_size = 0;

  delete[] m_data          ;
           m_data = nullptr;
}




}




