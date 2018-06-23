#ifndef stage_hpp
#define stage_hpp


#include"libgbact/character.hpp"
#include<memory>




namespace gbact{
namespace stages{


extern std::vector<square_data>
g_square_data_set;


enum class
prop_kind
{
  block,
  player,
  item,

};


class
prop
{
  prop_kind  m_kind;

  int  m_index;

public:
  constexpr prop(prop_kind  k=prop_kind::block, int  i=0) noexcept:
  m_kind(k), m_index(i){}

  constexpr bool  is_block()  const noexcept{return m_kind == prop_kind::block;}
  constexpr bool  is_player() const noexcept{return m_kind == prop_kind::player;}
  constexpr bool  is_item()   const noexcept{return m_kind == prop_kind::item;}

  constexpr int  get_index() const noexcept{return m_index;}

};


class
stage
{
  int  m_width =0;
  int  m_height=0;

  std::vector<prop>  m_table;

  prop*  m_lady_prop=nullptr;
  prop*  m_boy_prop=nullptr;

public:
  stage() noexcept{}
  stage(int  w, int  h) noexcept{resize(w,h);}

  void  resize(int  w, int  h) noexcept;

  int  get_width()  const noexcept{return m_width ;}
  int  get_height() const noexcept{return m_height;}

        prop&  get_prop(int  x, int  y)       noexcept{return m_table[(m_width*y)+x];}
  const prop&  get_prop(int  x, int  y) const noexcept{return m_table[(m_width*y)+x];}

  const prop*  get_lady_prop() const noexcept{return m_lady_prop;}
  const prop*  get_boy_prop() const noexcept{return m_boy_prop;}

  void  set_prop(const prop&  pr, int  x, int  y) noexcept;

};



}
}




#endif




