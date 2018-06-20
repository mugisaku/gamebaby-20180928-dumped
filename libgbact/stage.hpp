#ifndef stage_hpp
#define stage_hpp


#include"libgbact/character.hpp"
#include<memory>




namespace gbact{
namespace stages{


extern std::vector<square_data>
g_square_data_set;


class
stage
{
  int  m_width =0;
  int  m_height=0;

  std::vector<int>  m_table;

public:
  stage() noexcept{}
  stage(int  w, int  h) noexcept{resize(w,h);}

  void  resize(int  w, int  h) noexcept;

  int  get_width()  const noexcept{return m_width ;}
  int  get_height() const noexcept{return m_height;}

  int   get_index(        int  x, int  y) const noexcept{return m_table[(m_width*y)+x]    ;}
  void  set_index(int  i, int  x, int  y)       noexcept{       m_table[(m_width*y)+x] = i;}

};



}
}




#endif




