#ifndef stage_hpp
#define stage_hpp


#include"libgbact/character.hpp"
#include<memory>




namespace gbact{
namespace stages{


extern std::vector<square_data>
g_square_data_set;


class
prop
{
  int   m_block_index=0;
  int  m_object_index=0;

public:
  prop() noexcept{}
  prop(int  blki, int  obji) noexcept: m_block_index(blki), m_object_index(obji){}

  int  get_block_index() const noexcept{return m_block_index;}
  int  get_object_index() const noexcept{return m_object_index;}

  void  set_block_index(int  i) noexcept{m_block_index = i;}
  void  set_object_index(int  i) noexcept{m_object_index = i;}

};


class
stage
{
  int  m_width =0;
  int  m_height=0;

  std::vector<prop>  m_table;

  prop*  m_lady_prop=nullptr;
  prop*  m_boy_prop=nullptr;

  point  m_lady_point;
  point   m_boy_point;

public:
  stage() noexcept;

  void  resize(int  w, int  h) noexcept;

  int  get_width()  const noexcept{return m_width ;}
  int  get_height() const noexcept{return m_height;}

        prop&  get_prop(int  x, int  y)       noexcept{return m_table[(m_width*y)+x];}
  const prop&  get_prop(int  x, int  y) const noexcept{return m_table[(m_width*y)+x];}

  const prop*  get_lady_prop() const noexcept{return m_lady_prop;}
  const prop*  get_boy_prop() const noexcept{return m_boy_prop;}

  point  get_lady_point() const noexcept{return m_lady_point;}
  point  get_boy_point() const noexcept{return m_boy_point;}

  void  set_prop(const prop&  pr, int  x, int  y) noexcept;

  void  set_block_index(int  i, int  x, int  y) noexcept;
  void  set_object(     int  i, int  x, int  y) noexcept;

  void  restore(character_set&  chset) const noexcept;

};



}


using prop = stages::prop;
using stage = stages::stage;


}




#endif




