#ifndef GMBB_IMAGE_HPP
#define GMBB_IMAGE_HPP


#include<cstdint>
#include<vector>
#include<string>
#include"libgbstd/figures.hpp"
#include"libgbstd/binary_stream.hpp"
#include"libgbstd/ro_ptr.hpp"




namespace gbstd{
namespace images{


struct
color_index
{
  uint16_t  code;

  constexpr color_index(uint16_t  code_=0) noexcept: code(code_){}
  constexpr color_index(int  ri, int  gi, int  bi) noexcept: code(01000|(ri<<6)|(gi<<3)|bi){}

  constexpr operator bool() const noexcept{return code>>9;}

  constexpr int  get_r() const noexcept{return (code>>6)&7;}
  constexpr int  get_g() const noexcept{return (code>>3)&7;}
  constexpr int  get_b() const noexcept{return (code   )&7;}

  constexpr int  get_r255() const noexcept{return (((code>>6)&07)<<5)|0b11111;}
  constexpr int  get_g255() const noexcept{return (((code>>3)&07)<<5)|0b11111;}
  constexpr int  get_b255() const noexcept{return (((code   )&07)<<5)|0b11111;}

};


struct
predefined
{
  static constexpr color_index        null = color_index(00000);
  static constexpr color_index       white = color_index(01777);
  static constexpr color_index       black = color_index(01000);
  static constexpr color_index        gray = color_index(01333);
  static constexpr color_index  light_gray = color_index(01555);
  static constexpr color_index   dark_gray = color_index(01111);
  static constexpr color_index         red = color_index(01700);
  static constexpr color_index       green = color_index(01070);
  static constexpr color_index        blue = color_index(01007);
  static constexpr color_index      yellow = color_index(01770);
};


class
palette
{
  uint32_t  m_color_table[01000];

public:
  void  set_color(color_index  i, uint32_t  value) noexcept{m_color_table[i.code&0777] = value;}

  uint32_t  get_color(color_index  i) const noexcept{return m_color_table[i.code&0777];};

};


struct
pixel
{
  images::color_index  color_index=0;

  uint16_t  z=0;

  constexpr pixel(images::color_index  i=images::color_index(), uint16_t  z_=0) noexcept:
  color_index(i), z(z_){}

};


constexpr int  font_width  =  8;
constexpr int  font_height = 16;

class
text_style
{
  color_index  m_colors[4];

public:
  text_style(color_index  c0=0, color_index  c1=predefined::white, color_index  c2=0, color_index  c3=0) noexcept:
  m_colors{c0,c1,c2,c3}{}

  void         set_color_index(int  i, color_index  ci)       noexcept{       m_colors[i] = ci;}
  color_index  get_color_index(int  i                 ) const noexcept{return m_colors[i]     ;}

};



class
image
{
  int  m_width =0;
  int  m_height=0;

  std::vector<pixel>  m_pixels;

public:
  image() noexcept{}
  image(int  w, int  h, std::initializer_list<color_index>  ls={}) noexcept;


  void  resize(int  w, int  h) noexcept;

  void  load_png(binary_stream_reader&  r) noexcept;

  int   get_width() const noexcept{return m_width ;}
  int  get_height() const noexcept{return m_height;}

        pixel&        get_pixel(int  x, int  y)       noexcept{return m_pixels[m_width*y+x];}
  const pixel&  get_const_pixel(int  x, int  y) const noexcept{return m_pixels[m_width*y+x];}

  void  set_pixel(pixel  pix, int  x, int  y                ) noexcept{get_pixel(x,y) = pix;}
  void  set_pixel(pixel  pix, int  x, int  y, int  w, int  h) noexcept;

  uint32_t  get_pixel_color(int  x, int  y, palette const&  pal) const noexcept;


  void  fill(pixel  pix=pixel()) noexcept;

  void  add_rgb(int  r, int  g, int  b) noexcept;
  void  reverse_rgb() noexcept;


  void  draw_dot(       color_index  i, int  x, int  y) noexcept;
  void  draw_dot_safely(color_index  i, int  x, int  y) noexcept;

  void  draw_vline(       color_index  i, int  x, int  y, int  l) noexcept;
  void  draw_vline_safely(color_index  i, int  x, int  y, int  l) noexcept;
  void  draw_hline(       color_index  i, int  x, int  y, int  l, int  interval=0) noexcept;
  void  draw_hline_safely(color_index  i, int  x, int  y, int  l) noexcept;

  void  draw_line(color_index  i, int  x0, int  y0, int  x1, int  y1) noexcept;

  void  draw_rectangle(       color_index  i, int  x, int  y, int  w, int  h) noexcept;
  void  draw_rectangle_safely(color_index  i, int  x, int  y, int  w, int  h) noexcept;
  void  fill_rectangle(       color_index  i, int  x, int  y, int  w, int  h) noexcept;

  void  draw_doubleline_rectangle(color_index  in, color_index  out, int  x, int  y, int  w, int  h) noexcept;

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

  uint32_t  get_pixel_color(int  x, int  y, palette const&  pal) const noexcept{return m_image->get_pixel_color(get_x(x),get_y(y),pal);}


  void  draw_dot(       color_index  i, int  x, int  y) const noexcept{m_image->draw_dot(       i,get_x(x),get_y(y));}
  void  draw_dot_safely(color_index  i, int  x, int  y) const noexcept{m_image->draw_dot_safely(i,get_x(x),get_y(y));}

  void  draw_vline(       color_index  i, int  x, int  y, int  l) const noexcept{m_image->draw_vline(                 i,get_x(x),get_y(y),l);}
  void  draw_vline_safely(color_index  i, int  x, int  y, int  l) const noexcept{m_image->draw_vline_safely(          i,get_x(x),get_y(y),l);}
  void  draw_hline(       color_index  i, int  x, int  y, int  l, int  interval=0) const noexcept{m_image->draw_hline(i,get_x(x),get_y(y),l,interval);}
  void  draw_hline_safely(color_index  i, int  x, int  y, int  l) const noexcept{m_image->draw_hline_safely(          i,get_x(x),get_y(y),l);}

  void  draw_line(color_index  i, int  x0, int  y0, int  x1, int  y1) const noexcept{m_image->draw_line(i,get_x(x0),get_y(y0),get_x(x1),get_y(y1));}

  void  draw_rectangle(       color_index  i, int  x, int  y, int  w, int  h) const noexcept{m_image->draw_rectangle(       i,get_x(x),get_y(y),w,h);}
  void  draw_rectangle_safely(color_index  i, int  x, int  y, int  w, int  h) const noexcept{m_image->draw_rectangle_safely(i,get_x(x),get_y(y),w,h);}
  void  fill_rectangle(       color_index  i, int  x, int  y, int  w, int  h) const noexcept{m_image->fill_rectangle(       i,get_x(x),get_y(y),w,h);}

  void  draw_doubleline_rectangle(color_index  in, color_index  out, int  x, int  y, int  w, int  h) const noexcept{
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


using images::color_index;
using images::palette;
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




