#ifndef GMBB_IMAGE_HPP
#define GMBB_IMAGE_HPP


#include<cstdint>
#include<vector>
#include<string>
#include"libgbstd/figures.hpp"
#include"libgbstd/color.hpp"
#include"libgbstd/style.hpp"
#include"libgbstd/string.hpp"




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

  void  load_from_image_data(const uint8_t*  data) noexcept;

  std::vector<uint8_t>  make_image_data() const noexcept;

  void  load_from_webp(const uint8_t*  data, size_t  data_size) noexcept;
  void  load_from_webp(const char*  filepath)       noexcept;
  void    save_to_webp(const char*  filepath) const noexcept;

  void  load_from_png(const uint8_t*  data, size_t  data_size) noexcept;
  void  load_from_png(const char*  filepath)       noexcept;
  void    save_to_png(const char*  filepath) const noexcept;

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
  void  fill_rectangle_safely(color  i, int  x, int  y, int  w, int  h) noexcept;

  void  draw_doubleline_rectangle(color  in, color  out, int  x, int  y, int  w, int  h) noexcept;

  void  draw_stripe_rectangle(color  first, color  second, int  interval, int  x, int  y, int  w, int  h) noexcept;

  void  draw_character(char16_t           c, const text_style&  style, int  x, int  y) noexcept;
  void  draw_text(gbstd::string_view     sv, const text_style&  style, int  x, int  y) noexcept;
  void  draw_text(gbstd::u16string_view  sv, const text_style&  style, int  x, int  y) noexcept;

  void  draw_character_safely(char16_t           c, const text_style&  style, int  x, int  y) noexcept;
  void  draw_text_safely(gbstd::string_view     sv, const text_style&  style, int  x, int  y) noexcept;
  void  draw_text_safely(gbstd::u16string_view  sv, const text_style&  style, int  x, int  y) noexcept;

};


class
image_cursor
{
  image*  m_image;

  point  m_offset;

  int  z_value=0;

  int  get_x(int  x) const noexcept{return m_offset.x+x;}
  int  get_y(int  y) const noexcept{return m_offset.y+y;}

public:
  image_cursor() noexcept:
  m_image(nullptr),
  m_offset(0,0){}

  image_cursor(image&  image) noexcept:
  m_image(&image),
  m_offset(0,0){}

  image_cursor(image&  image, point  offset) noexcept:
  m_image(&image),
  m_offset(offset){}


  operator bool() const noexcept{return m_image;}

  image_cursor  operator+(point  offset) const noexcept{return image_cursor(*m_image,m_offset+offset);}

  image&  get_image() const noexcept{return *m_image;}

  void  set_offset(int  x, int  y) noexcept{m_offset  = point(x,y);}
  void  add_offset(int  x, int  y) noexcept{m_offset += point(x,y);}

  const point&  get_offset() const noexcept{return m_offset;}

        pixel&        get_pixel(int  x, int  y) const noexcept{return m_image->get_pixel(      get_x(x),get_y(y));}
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
void    paste(const image&  src, rectangle  src_rect, image&        dst, point  dst_pt) noexcept;
void    paste(const image&  src, rectangle  src_rect, image_cursor  dst               ) noexcept;
void  overlay(const image&  src, rectangle  src_rect, image&        dst, point  dst_pt) noexcept;
void  overlay(const image&  src, rectangle  src_rect, image_cursor  dst               ) noexcept;





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


  std::vector<dot>  m_dot_buffer;

  struct record;

  record*  m_record_list=nullptr;

  uint32_t  m_number_of_records=0;

public:
  drawing_recorder() noexcept{}
  drawing_recorder(const drawing_recorder&   rhs) noexcept=delete;
  drawing_recorder(      drawing_recorder&&  rhs) noexcept{*this = std::move(rhs);}
 ~drawing_recorder(){clear();}

  drawing_recorder&  operator=(const drawing_recorder&   rhs) noexcept=delete;
  drawing_recorder&  operator=(      drawing_recorder&&  rhs) noexcept;

  void  put(colors::color  color, int  x, int  y) noexcept{m_dot_buffer.emplace_back(color,x,y);}

  bool  rollback(image&  img) noexcept;

  void  clear() noexcept;

  bool  push(bool  solid) noexcept;

  uint32_t  get_number_of_records() const noexcept{return m_number_of_records;}

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
  line_maker(int  x0=0, int  y0=0, int  x1=0, int  y1=0) noexcept{reset(x0,y0,x1,y1);}
  line_maker(point  a, point  b) noexcept{reset(a,b);}

  void  reset(int  x0=0, int  y0=0, int  x1=0, int  y1=0) noexcept;
  void  reset(point  a, point  b) noexcept{reset(a.x,a.y,b.x,b.y);}

  int  get_distance() const noexcept{return m_distance;}

  int  get_x() const noexcept{return m_x>>shift_amount;}
  int  get_y() const noexcept{return m_y>>shift_amount;}

  void  step() noexcept;

};


struct
sprite
{
  const images::image*  src_image=nullptr;

  rectangle  src_rectangle;

  point  dst_point;

  void  render(images::image&  dst) const noexcept
  {
      if(src_image)
      {
        images::overlay(*src_image,src_rectangle,dst,dst_point);
      }
  }

};


}


using images::font_width;
using images::font_height;

using images::image;
using images::image_cursor;
using images::drawing_recorder;
using images::pixel;
using images::line_maker;
using images::sprite;


}




#endif




