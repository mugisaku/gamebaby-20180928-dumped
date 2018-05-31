#include"libgbstd/board.hpp"




namespace gbstd{
namespace boards{




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


void
area::
print() const noexcept
{
  printf("area:{top:%4d,left:%4d,right:%4d,bottom:%4d}\n",top,left,right,bottom);
}




}}




