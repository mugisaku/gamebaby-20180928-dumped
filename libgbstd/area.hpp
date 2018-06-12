#ifndef area_HPP
#define area_HPP


#include<cstdio>


namespace gbstd{
namespace areas{




struct
area
{
  int     top=0;
  int    left=0;
  int   right=0;
  int  bottom=0;


  static constexpr bool  test_x_collision(const area&  a, const area&  b) noexcept
  {
      if(a.left < b.left){return a.right >= b.left;}
    else                 {return b.right >= a.left;}
  }


  static constexpr bool  test_y_collision(const area&  a, const area&  b) noexcept
  {
      if(a.top < b.top){return a.bottom >= b.top;}
    else               {return b.bottom >= a.top;}
  }


  static constexpr bool  test_collision(const area&  a, const area&  b) noexcept
  {
    return(test_x_collision(a,b) &&
           test_y_collision(a,b));
  }

  void  print() const noexcept
  {
    printf("area:{top:%4d,left:%4d,right:%4d,bottom:%4d}\n",top,left,right,bottom);
  }

};


}


using areas::area;


}




#endif




