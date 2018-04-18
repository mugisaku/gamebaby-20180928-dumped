#include"libgbstd/image.hpp"



namespace gbstd{
namespace images{




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


const icon
predefined_icon::
left
{
#include"libgbstd/images/icon_data__left.txt"
};


const icon
predefined_icon::
sunken_left
{
#include"libgbstd/images/icon_data__sunken_left.txt"
};


const icon
predefined_icon::
right
{
#include"libgbstd/images/icon_data__right.txt"
};


const icon
predefined_icon::
sunken_right
{
#include"libgbstd/images/icon_data__sunken_right.txt"
};


const icon
predefined_icon::
checked
{
#include"libgbstd/images/icon_data__checked.txt"
};


const icon
predefined_icon::
unchecked
{
#include"libgbstd/images/icon_data__unchecked.txt"
};


const icon
predefined_icon::
radio_checked
{
#include"libgbstd/images/icon_data__radio_checked.txt"
};


const icon
predefined_icon::
radio_unchecked
{
#include"libgbstd/images/icon_data__radio_unchecked.txt"
};


}}




