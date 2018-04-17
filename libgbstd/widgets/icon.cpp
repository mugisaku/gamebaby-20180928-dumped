#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




icon::
icon(std::initializer_list<int>  ls) noexcept
{
  auto   it = &m_data[   0][0];
  auto  end = &m_data[size][0];

    for(auto  i: ls)
    {
      *it++ = color(i);
    }
}


icon::
icon(std::initializer_list<color>  ls) noexcept
{
  auto   it = &m_data[   0][0];
  auto  end = &m_data[size][0];

    for(auto  i: ls)
    {
      *it++ = i;
    }
}




void
icon::
print() const noexcept
{
    for(int  y = 0;  y < size;  ++y)
    {
        for(int  x = 0;  x < size;  ++x)
        {
          m_data[y][x].print();

          printf(",");
        }


      printf("\n");
    }
}


}}




