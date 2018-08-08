#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




image_data&
image_data::
assign(const std::vector<const binary*>&  ls) noexcept
{
  size_t  size = 0;

    for(auto  bin: ls)
    {
      size += bin->get_data_size();
    }


  resize(size);

  auto  p = begin();

    for(auto  bin: ls)
    {
      auto  current_size = bin->get_data_size();

      std::memcpy(p,bin->begin(),current_size);

      p += current_size;
    }


  return *this;
}




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




