#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{





bool
file_wrapper::
open_ro(const char*  path) noexcept
{
  close();

  m_file = fopen(path,"rb");

  return m_file;
}


bool
file_wrapper::
open_rw(const char*  path) noexcept
{
  close();

  m_file = fopen(path,"wb");

  return m_file;
}


void
file_wrapper::
close() noexcept
{
    if(m_file)
    {
      fclose(m_file)         ;
             m_file = nullptr;
    }
}




}




