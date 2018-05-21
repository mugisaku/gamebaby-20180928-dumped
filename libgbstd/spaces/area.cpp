#include"libgbstd/space.hpp"




namespace gbstd{
namespace spaces{




bool
area::
test_x_collision(const area&  a, const area&  b) noexcept
{
    if(a.left < b.left){return a.right >= b.left;}
  else                 {return b.right >= a.left;}
}


bool
area::
test_y_collision(const area&  a, const area&  b) noexcept
{
    if(a.top < b.top){return a.bottom >= b.top;}
  else               {return b.bottom >= a.top;}
}


bool
area::
test_collision(const area&  a, const area&  b) noexcept
{
  return(test_x_collision(a,b) &&
         test_y_collision(a,b));
}


}}




