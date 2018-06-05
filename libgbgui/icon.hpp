#ifndef gbgui_icon_hpp
#define gbgui_icon_hpp


#include"libgbstd/color.hpp"
#include<initializer_list>




namespace gbgui{
namespace icons{


using namespace gbstd;


class
icon
{
public:
  static constexpr int  size = 16;

private:
  color  m_data[size][size]={0};

public:
  icon(std::initializer_list<int>  ls) noexcept;
  icon(std::initializer_list<color>  ls) noexcept;

  void   set_color(int  x, int  y, color  i)       noexcept{       m_data[y][x] = i;}
  color  get_color(int  x, int  y          ) const noexcept{return m_data[y][x]    ;}

  void  print() const noexcept;

};


extern const icon    checked;
extern const icon  unchecked;

extern const icon    radio_checked;
extern const icon  radio_unchecked;

//  static const icon         up;
//  static const icon  sunken_up;

extern const icon         left;
extern const icon  sunken_left;

extern const icon         right;
extern const icon  sunken_right;

//  static const icon         down;
//  static const icon  sunken_down;

}


using icon = icons::icon;


}




#endif




