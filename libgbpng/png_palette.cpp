#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{


palette::
palette(const chunk&  chk) noexcept
{
  auto  it     = std::begin(m_colors);
  auto  it_end = std::end(m_colors);

  auto  src = chk.get_data();

  m_number_of_colors = chk.get_data_size()/3;

    for(int  i = 0;  i < m_number_of_colors;  ++i)
    {
      it->r = *src++;
      it->g = *src++;
      it->b = *src++;

      ++it;
    }


    while(it != it_end)
    {
      it->r = 0;
      it->g = 0;
      it->b = 0;

      ++it;
    }
}




chunk
palette::
make_chunk() const noexcept
{
  return chunk(binary(&m_colors,3*m_number_of_colors),chunk_name("PLTE"));
}




void
palette::
print() const noexcept
{
}


}




