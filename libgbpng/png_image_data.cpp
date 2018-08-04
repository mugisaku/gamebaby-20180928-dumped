#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{


image_data::
image_data(const chunk&  chk) noexcept
{
}




chunk
image_data::
make_chunk() const noexcept
{
  return chunk(m_data_size,chunk_name("IDAT"),m_data);
}




void
image_data::
print() const noexcept
{
}


}




