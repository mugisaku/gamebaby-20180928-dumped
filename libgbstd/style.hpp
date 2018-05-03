#ifndef gbstd_style_hpp
#define gbstd_style_hpp


#include"libgbstd/color.hpp"




namespace gbstd{
namespace styles{


class
text_style
{
  color  m_colors[4];

public:
  constexpr text_style() noexcept{}

  constexpr text_style(color  c0, color  c1, color  c2, color  c3) noexcept:
  m_colors{c0,c1,c2,c3}{}

  void             set_color(int  i, color  ci)       noexcept{       m_colors[i] = ci;}
  constexpr color  get_color(int  i           ) const noexcept{return m_colors[i]     ;}

};


constexpr text_style  a_black_based_text_style(colors::white,colors::black,colors::light_gray,colors::gray);
constexpr text_style  b_black_based_text_style(colors::null ,colors::black,colors::light_gray,colors::gray);
constexpr text_style  a_white_based_text_style(colors::black,colors::white,colors::light_gray,colors::gray);
constexpr text_style  b_white_based_text_style(colors::null ,colors::white,colors::light_gray,colors::gray);


class
background_style
{
  enum class kind{
    single_color,
    stripe,
  } m_kind=kind::single_color;

  color   m_first_color;
  color  m_second_color;

  int  m_interval;

public:
  background_style() noexcept{}
  background_style(color  color1) noexcept: m_first_color(color1){}

  background_style(color  color1, color  color2, int  interval) noexcept:
  m_kind(kind::stripe), m_first_color(color1), m_second_color(color2), m_interval(interval){}

  void  be_single_color() noexcept{m_kind = kind::single_color;}
  void  be_stripe()       noexcept{m_kind = kind::stripe;}

  bool  is_single_color() const noexcept{return m_kind == kind::single_color;}
  bool  is_stripe() const noexcept{return m_kind == kind::stripe;}

  void  set_interval(int  v)       noexcept{       m_interval = v;}
  int   get_interval(      ) const noexcept{return m_interval    ;}

  void    set_first_color(colors::color  color)       noexcept{       m_first_color = color;}
  color   get_first_color(                    ) const noexcept{return m_first_color        ;}

  void   set_second_color(colors::color  color)       noexcept{       m_second_color = color;}
  color  get_second_color(                    ) const noexcept{return m_second_color        ;}

};


}


using styles::text_style;
using styles::background_style;


}




#endif




