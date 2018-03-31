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

  int16_t  z=0;

  constexpr pixel(images::color_index  i=images::color_index(), int16_t  z_=0) noexcept:
  color_index(i), z(z_){}

};


class
text_style
{
  color_index  m_colors[4]={0,predefined::white,0,0};

public:
  void         set_color_index(int  i, color_index  ci)       noexcept{       m_colors[i] = ci;}
  color_index  get_color_index(int  i                 ) const noexcept{return m_colors[i]     ;}

};



class
image
{
  int  m_width;
  int  m_height;

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

  uint32_t  get_pixel_color(int  x, int  y, palette const&  pal) const noexcept;


  void  fill(pixel  pix=pixel()) noexcept;

  void  add_rgb(int  r, int  g, int  b) noexcept;
  void  reverse_rgb() noexcept;


  void  draw_dot(       color_index  i, int  x, int  y) noexcept;
  void  draw_dot_safely(color_index  i, int  x, int  y) noexcept;

  void  draw_dot(       pixel  pix, int  x, int  y) noexcept;
  void  draw_dot_safely(pixel  pix, int  x, int  y) noexcept;

  void  draw_vline(       pixel  pix, int  x, int  y, int  l) noexcept;
  void  draw_vline_safely(pixel  pix, int  x, int  y, int  l) noexcept;
  void  draw_hline(       pixel  pix, int  x, int  y, int  l, int  interval=0) noexcept;
  void  draw_hline_safely(pixel  pix, int  x, int  y, int  l) noexcept;

  void  draw_line(pixel  pix, int  x0, int  y0, int  x1, int  y1) noexcept;

  void  draw_rectangle(       pixel  pix, int  x, int  y, int  w, int  h) noexcept;
  void  draw_rectangle_safely(pixel  pix, int  x, int  y, int  w, int  h) noexcept;
  void  fill_rectangle(       pixel  pix, int  x, int  y, int  w, int  h) noexcept;

  void  draw_doubleline_rectangle(pixel  in, pixel  out, int  x, int  y, int  w, int  h) noexcept;

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

  image_cursor(const image_cursor&  src, int  x, int  y) noexcept:
  m_image(src.m_image),
  m_x_offset(src.m_x_offset+x),
  m_y_offset(src.m_y_offset+y){}


  image_cursor  operator+(point  offset) const noexcept;

  image&  get_image() const noexcept{return *m_image;}

  int  get_x_offset() const noexcept{return m_x_offset;}
  int  get_y_offset() const noexcept{return m_y_offset;}

  void  set_offset(int  x, int  y) noexcept;
  void  add_offset(int  x, int  y) noexcept;

        pixel&        get_pixel(int  x, int  y) const noexcept{return m_image->get_pixel(get_x(x),get_y(y));}
  const pixel&  get_const_pixel(int  x, int  y) const noexcept{return m_image->get_const_pixel(get_x(x),get_y(y));}

  uint32_t  get_pixel_color(int  x, int  y, palette const&  pal) const noexcept{return m_image->get_pixel_color(get_x(x),get_y(y),pal);}


  void  draw_dot(       color_index  i, int  x, int  y) const noexcept{m_image->draw_dot(       i,get_x(x),get_y(y));}
  void  draw_dot_safely(color_index  i, int  x, int  y) const noexcept{m_image->draw_dot_safely(i,get_x(x),get_y(y));}

  void  draw_dot(       pixel  pix, int  x, int  y) const noexcept{m_image->draw_dot(       pix,get_x(x),get_y(y));}
  void  draw_dot_safely(pixel  pix, int  x, int  y) const noexcept{m_image->draw_dot_safely(pix,get_x(x),get_y(y));}

  void  draw_vline(       pixel  pix, int  x, int  y, int  l) const noexcept{m_image->draw_vline(                 pix,get_x(x),get_y(y),l);}
  void  draw_vline_safely(pixel  pix, int  x, int  y, int  l) const noexcept{m_image->draw_vline_safely(          pix,get_x(x),get_y(y),l);}
  void  draw_hline(       pixel  pix, int  x, int  y, int  l, int  interval=0) const noexcept{m_image->draw_hline(pix,get_x(x),get_y(y),l,interval);}
  void  draw_hline_safely(pixel  pix, int  x, int  y, int  l) const noexcept{m_image->draw_hline_safely(          pix,get_x(x),get_y(y),l);}

  void  draw_line(pixel  pix, int  x0, int  y0, int  x1, int  y1) const noexcept{m_image->draw_line(pix,get_x(x0),get_y(y0),get_x(x1),get_y(y1));}

  void  draw_rectangle(       pixel  pix, int  x, int  y, int  w, int  h) const noexcept{m_image->draw_rectangle(       pix,get_x(x),get_y(y),w,h);}
  void  draw_rectangle_safely(pixel  pix, int  x, int  y, int  w, int  h) const noexcept{m_image->draw_rectangle_safely(pix,get_x(x),get_y(y),w,h);}
  void  fill_rectangle(       pixel  pix, int  x, int  y, int  w, int  h) const noexcept{m_image->fill_rectangle(       pix,get_x(x),get_y(y),w,h);}

  void  draw_doubleline_rectangle(pixel  in, pixel  out, int  x, int  y, int  w, int  h) const noexcept{
    m_image->draw_doubleline_rectangle(in,out,get_x(x),get_y(y),w,h);}

  void  draw_character(char16_t           c, const text_style&  style, int  x, int  y) const noexcept{m_image->draw_character(c,style,get_x(x),get_y(y));}
  void  draw_text(gbstd::string_view     sv, const text_style&  style, int  x, int  y) const noexcept{m_image->draw_text(sv,style,get_x(x),get_y(y));}
  void  draw_text(gbstd::u16string_view  sv, const text_style&  style, int  x, int  y) const noexcept{m_image->draw_text(sv,style,get_x(x),get_y(y));}

};


class
image_frame: public image_cursor
{
  int  m_width;
  int  m_height;

public:
  image_frame(image&  image, point  pt=point(), int  w=0, int  h=0) noexcept:
  image_cursor(image,pt),
  m_width(w? w:image.get_width()),
  m_height(h? h:image.get_height()){}


  int  get_width()  const noexcept{return m_width;}
  int  get_height() const noexcept{return m_height;}

  void  fill(pixel  pix=pixel()) const noexcept;

  void  add(int  r, int  g, int  b) const noexcept;
  void  reverse_color() const noexcept;

};


void  transfer(const image_frame&  src, const image_cursor&  dst        ) noexcept;
void  transfer(const image_frame&  src, const image_cursor&  dst, int  z) noexcept;


}


using images::color_index;
using images::palette;
using images::text_style;
using images::predefined;

using images::image;
using images::image_cursor;
using images::image_frame;
using images::pixel;


}




#endif




