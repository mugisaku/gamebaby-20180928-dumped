#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




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


}




