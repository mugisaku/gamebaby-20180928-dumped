#ifndef GMBB_IMAGE_HPP
#define GMBB_IMAGE_HPP


#include<cstdint>
#include<vector>
#include<string>
#include"libgbstd/figures.hpp"
#include"libgbstd/ios/binary_stream.hpp"
#include"libgbstd/ro_ptr.hpp"




namespace gbstd{
namespace images{


struct
color
{
  uint16_t  code;

  static constexpr int  masked(int  v) noexcept{return v&7;}

  constexpr color(int  v=0) noexcept: code((v&01000)? color(v>>6,v>>3,v).code:0){}
  constexpr color(int  r7, int  g7, int  b7) noexcept: code(0x8000|((masked(r7)<<2)<<10)|
                                                                   ((masked(g7)<<2)<< 5)|
                                                                   ((masked(b7)<<2)    )){}

  constexpr operator bool() const noexcept{return code>>15;}

  constexpr int  get_r7() const noexcept{return masked(((code>>10)>>2));}
  constexpr int  get_g7() const noexcept{return masked(((code>> 5)>>2));}
  constexpr int  get_b7() const noexcept{return masked(((code    )>>2));}

  constexpr int  get_r255() const noexcept{return (get_r7()<<5)|0b11111;}
  constexpr int  get_g255() const noexcept{return (get_g7()<<5)|0b11111;}
  constexpr int  get_b255() const noexcept{return (get_b7()<<5)|0b11111;}

};


struct
predefined
{
  static constexpr color       white = color(7,7,7);
  static constexpr color       black = color(0,0,0);
  static constexpr color        gray = color(3,3,3);
  static constexpr color  light_gray = color(5,5,5);
  static constexpr color   dark_gray = color(1,1,1);
  static constexpr color         red = color(7,0,0);
  static constexpr color       green = color(0,7,0);
  static constexpr color        blue = color(0,0,7);
  static constexpr color      yellow = color(7,7,0);
};


struct
pixel
{
  images::color  color=0;

  uint16_t  z=0;

  constexpr pixel(images::color  i=images::color(), uint16_t  z_=0) noexcept:
  color(i), z(z_){}

};


constexpr int  font_width  =  8;
constexpr int  font_height = 16;

class
text_style
{
  color  m_colors[4];

public:
  text_style(color  c0=0, color  c1=predefined::white, color  c2=0, color  c3=0) noexcept:
  m_colors{c0,c1,c2,c3}{}

  void         set_color(int  i, color  ci)       noexcept{       m_colors[i] = ci;}
  color  get_color(int  i                 ) const noexcept{return m_colors[i]     ;}

};



class
image
{
  int  m_width =0;
  int  m_height=0;

  std::vector<pixel>  m_pixels;

public:
  image() noexcept{}
  image(int  w, int  h, std::initializer_list<int>  ls) noexcept;
  image(int  w, int  h, std::initializer_list<color>  ls={}) noexcept;


  void  resize(int  w, int  h) noexcept;

  void  load_png(binary_stream_reader&  r) noexcept;

  int   get_width() const noexcept{return m_width ;}
  int  get_height() const noexcept{return m_height;}

        pixel&        get_pixel(int  x, int  y)       noexcept{return m_pixels[m_width*y+x];}
  const pixel&  get_const_pixel(int  x, int  y) const noexcept{return m_pixels[m_width*y+x];}

  void  set_pixel(pixel  pix, int  x, int  y                ) noexcept{get_pixel(x,y) = pix;}
  void  set_pixel(pixel  pix, int  x, int  y, int  w, int  h) noexcept;


  void  fill(pixel  pix=pixel()) noexcept;

  void  add_rgb(int  r, int  g, int  b) noexcept;
  void  reverse_rgb() noexcept;


  void  draw_dot(       color  i, int  x, int  y) noexcept;
  void  draw_dot_safely(color  i, int  x, int  y) noexcept;

  void  draw_vline(       color  i, int  x, int  y, int  l) noexcept;
  void  draw_vline_safely(color  i, int  x, int  y, int  l) noexcept;
  void  draw_hline(       color  i, int  x, int  y, int  l, int  interval=0) noexcept;
  void  draw_hline_safely(color  i, int  x, int  y, int  l) noexcept;

  void  draw_line(color  i, int  x0, int  y0, int  x1, int  y1) noexcept;

  void  draw_rectangle(       color  i, int  x, int  y, int  w, int  h) noexcept;
  void  draw_rectangle_safely(color  i, int  x, int  y, int  w, int  h) noexcept;
  void  fill_rectangle(       color  i, int  x, int  y, int  w, int  h) noexcept;

  void  draw_doubleline_rectangle(color  in, color  out, int  x, int  y, int  w, int  h) noexcept;

  void  draw_character(char16_t           c, const text_style&  style, int  x, int  y) noexcept;
  void  draw_text(gbstd::string_view     sv, const text_style&  style, int  x, int  y) noexcept;
  void  draw_text(gbstd::u16string_view  sv, const text_style&  style, int  x, int  y) noexcept;

};


class
image_cursor
{
protected:
  image*  m_image;

  int  m_x_offset;
  int  m_y_offset;

  int  get_x(int  x) const noexcept{return m_x_offset+x;}
  int  get_y(int  y) const noexcept{return m_y_offset+y;}

public:
  image_cursor(image&  image) noexcept:
  m_image(&image),
  m_x_offset(0),
  m_y_offset(0){}

  image_cursor(image&  image, point  pt) noexcept:
  m_image(&image),
  m_x_offset(pt.x),
  m_y_offset(pt.y){}


  image_cursor  operator+(point  offset) const noexcept;

  image&  get_image() const noexcept{return *m_image;}

  int  get_x_offset() const noexcept{return m_x_offset;}
  int  get_y_offset() const noexcept{return m_y_offset;}

  void  set_offset(int  x, int  y) noexcept;
  void  add_offset(int  x, int  y) noexcept;

        pixel&        get_pixel(int  x, int  y) const noexcept{return m_image->get_pixel(get_x(x),get_y(y));}
  const pixel&  get_const_pixel(int  x, int  y) const noexcept{return m_image->get_const_pixel(get_x(x),get_y(y));}

  void  set_pixel(pixel  pix, int  x, int  y                ) noexcept{m_image->set_pixel(pix,get_x(x),get_y(y));}
  void  set_pixel(pixel  pix, int  x, int  y, int  w, int  h) noexcept{m_image->set_pixel(pix,get_x(x),get_y(y),w,h);}


  void  draw_dot(       color  i, int  x, int  y) const noexcept{m_image->draw_dot(       i,get_x(x),get_y(y));}
  void  draw_dot_safely(color  i, int  x, int  y) const noexcept{m_image->draw_dot_safely(i,get_x(x),get_y(y));}

  void  draw_vline(       color  i, int  x, int  y, int  l) const noexcept{m_image->draw_vline(                 i,get_x(x),get_y(y),l);}
  void  draw_vline_safely(color  i, int  x, int  y, int  l) const noexcept{m_image->draw_vline_safely(          i,get_x(x),get_y(y),l);}
  void  draw_hline(       color  i, int  x, int  y, int  l, int  interval=0) const noexcept{m_image->draw_hline(i,get_x(x),get_y(y),l,interval);}
  void  draw_hline_safely(color  i, int  x, int  y, int  l) const noexcept{m_image->draw_hline_safely(          i,get_x(x),get_y(y),l);}

  void  draw_line(color  i, int  x0, int  y0, int  x1, int  y1) const noexcept{m_image->draw_line(i,get_x(x0),get_y(y0),get_x(x1),get_y(y1));}

  void  draw_rectangle(       color  i, int  x, int  y, int  w, int  h) const noexcept{m_image->draw_rectangle(       i,get_x(x),get_y(y),w,h);}
  void  draw_rectangle_safely(color  i, int  x, int  y, int  w, int  h) const noexcept{m_image->draw_rectangle_safely(i,get_x(x),get_y(y),w,h);}
  void  fill_rectangle(       color  i, int  x, int  y, int  w, int  h) const noexcept{m_image->fill_rectangle(       i,get_x(x),get_y(y),w,h);}

  void  draw_doubleline_rectangle(color  in, color  out, int  x, int  y, int  w, int  h) const noexcept{
    m_image->draw_doubleline_rectangle(in,out,get_x(x),get_y(y),w,h);}

  void  draw_character(char16_t           c, const text_style&  style, int  x, int  y) const noexcept{m_image->draw_character(c,style,get_x(x),get_y(y));}
  void  draw_text(gbstd::string_view     sv, const text_style&  style, int  x, int  y) const noexcept{m_image->draw_text(sv,style,get_x(x),get_y(y));}
  void  draw_text(gbstd::u16string_view  sv, const text_style&  style, int  x, int  y) const noexcept{m_image->draw_text(sv,style,get_x(x),get_y(y));}

};


void  transfer(const image&  src, point  src_pt, int  src_w, int  src_h, const image_cursor&  dst        ) noexcept;
void  transfer(const image&  src, point  src_pt, int  src_w, int  src_h, const image_cursor&  dst, int  z) noexcept;


class
line_maker
{
  static constexpr int  shift_amount = 16;

  int  m_distance=0;

  int  m_x_add_amount=0;
  int  m_y_add_amount=0;

  int  m_x;
  int  m_y;

public:
  line_maker(int  x0=0, int  y0=0, int  x1=0, int  y1=0) noexcept;

  int  get_distance() const noexcept{return m_distance;}

  int  get_x() const noexcept{return m_x>>shift_amount;}
  int  get_y() const noexcept{return m_y>>shift_amount;}

  void  step() noexcept;

};


}


using images::color;
using images::text_style;
using images::predefined;
using images::font_width;
using images::font_height;

using images::image;
using images::image_cursor;
using images::pixel;
using images::line_maker;


}




#endif




