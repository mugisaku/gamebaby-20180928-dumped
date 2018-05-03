#ifndef GMBB_IMAGE_HPP
#define GMBB_IMAGE_HPP


#include<cstdint>
#include<vector>
#include<string>
#include"libgbstd/figures.hpp"
#include"libgbstd/ios/binary_stream.hpp"
#include"libgbstd/ro_ptr.hpp"
#include"libgbstd/color.hpp"
#include"libgbstd/style.hpp"




namespace gbstd{
namespace images{


struct
pixel
{
  colors::color  color=0;

  uint16_t  z=0;

  constexpr pixel(colors::color  i=colors::color(), uint16_t  z_=0) noexcept:
  color(i), z(z_){}

  constexpr operator colors::color() const noexcept{return color;}

};


constexpr int  font_width  =  8;
constexpr int  font_height = 16;


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

  rectangle  get_rectangle() const noexcept{return rectangle(0,0,m_width,m_height);}

  pixel*  begin() noexcept{return &m_pixels.front();}
  pixel*    end() noexcept{return &m_pixels.back()+1;}

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

  void  draw_stripe_rectangle(color  first, color  second, int  interval, int  x, int  y, int  w, int  h) noexcept;

  void  draw_character(char16_t           c, const text_style&  style, int  x, int  y) noexcept;
  void  draw_text(gbstd::string_view     sv, const text_style&  style, int  x, int  y) noexcept;
  void  draw_text(gbstd::u16string_view  sv, const text_style&  style, int  x, int  y) noexcept;

};


class
image_cursor
{
  image*  m_image;

  rectangle  m_rectangle;

  int  z_value=0;

  int  get_x(int  x) const noexcept{return m_rectangle.x+x;}
  int  get_y(int  y) const noexcept{return m_rectangle.y+y;}

public:
  image_cursor(image&  image) noexcept:
  m_image(&image),
  m_rectangle(0,0,image.get_width(),image.get_height()){}

  image_cursor(image&  image, point  pt) noexcept:
  m_image(&image),
  m_rectangle(pt,image.get_width(),image.get_height()){}


  image_cursor  operator+(point  offset) const noexcept;

  image&  get_image() const noexcept{return *m_image;}

  void  set_rectangle(int  x, int  y, int  w, int  h) noexcept{m_rectangle = rectangle(x,y,w,h);}
  rectangle&  get_rectangle() noexcept{return m_rectangle;}

  int  get_x_offset() const noexcept{return m_rectangle.x;}
  int  get_y_offset() const noexcept{return m_rectangle.y;}

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

  void  draw_stripe_rectangle(color  first, color  second, int  interval, int  x, int  y, int  w, int  h) const noexcept{
    m_image->draw_stripe_rectangle(first,second,interval,get_x(x),get_y(y),w,h);}

  void  draw_character(char16_t           c, const text_style&  style, int  x, int  y) const noexcept{m_image->draw_character(c,style,get_x(x),get_y(y));}
  void  draw_text(gbstd::string_view     sv, const text_style&  style, int  x, int  y) const noexcept{m_image->draw_text(sv,style,get_x(x),get_y(y));}
  void  draw_text(gbstd::u16string_view  sv, const text_style&  style, int  x, int  y) const noexcept{m_image->draw_text(sv,style,get_x(x),get_y(y));}

};


void  transform(rectangle&  src_rect, rectangle&  dst_rect) noexcept;
void  transfer(const image&  src, rectangle  src_rect, image_cursor&  dst, bool  layer) noexcept;





class
drawing_recorder
{
  struct dot{
    colors::color  color;
    uint16_t  x;
    uint16_t  y;

    dot(colors::color  color_=colors::color(), int  x_=0, int  y_=0) noexcept:
    color(color_), x(x_), y(y_){}

  };


  uint32_t  m_count=0;

  std::vector<dot>  m_dot_buffer;

  struct record;

  record*  m_record_list=nullptr;

public:
  drawing_recorder() noexcept{}
  drawing_recorder(const drawing_recorder&   rhs) noexcept=delete;
  drawing_recorder(      drawing_recorder&&  rhs) noexcept{*this = std::move(rhs);}
 ~drawing_recorder(){clear();}

  drawing_recorder&  operator=(const drawing_recorder&   rhs) noexcept=delete;
  drawing_recorder&  operator=(      drawing_recorder&&  rhs) noexcept;

  void  put(colors::color  color, int  x, int  y) noexcept{m_dot_buffer.emplace_back(color,x,y);}

  void  rollback(image&  img) noexcept;

  void  clear() noexcept;

  void  push(bool  solid) noexcept;

  uint32_t  get_count() const noexcept{return m_count;}

  void  reset_count() noexcept{m_count = 0;}

};


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


using images::font_width;
using images::font_height;

using images::image;
using images::image_cursor;
using images::drawing_recorder;
using images::pixel;
using images::line_maker;


}




#endif




