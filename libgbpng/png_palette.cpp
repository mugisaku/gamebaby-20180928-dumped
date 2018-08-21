#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




palette&
palette::
assign(const chunk&  chk) noexcept
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


  return *this;
}




int
palette::
find_color(const color&  color) const noexcept
{
    for(int  i = 0;  i < m_number_of_colors;  ++i)
    {
      auto&  current = m_colors[i];

        if((current.r == color.r) &&
           (current.g == color.g) &&
           (current.b == color.b))
        {
          return i;
        }
    }


  return -1;
}


int
palette::
append_color(const color&  color) noexcept
{
    if(m_number_of_colors < 256)
    {
        for(int  i = 0;  i < m_number_of_colors;  ++i)
        {
          auto&  current = m_colors[i];

            if((current.r == color.r) &&
               (current.g == color.g) &&
               (current.b == color.b))
            {
              return i;
            }
        }


      int  i = m_number_of_colors++;

      m_colors[i] = color;

      return i;
    }


  return -1;
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




