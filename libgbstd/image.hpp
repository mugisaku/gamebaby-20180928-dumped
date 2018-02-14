#ifndef GMBB_IMAGE_HPP
#define GMBB_IMAGE_HPP


#include<cstdint>
#include<vector>
#include<string>
#include"libgbstd/font.hpp"
#include"libgbstd/figures.hpp"
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

  constexpr int  get_r() const noexcept{return (code>>6)&07;}
  constexpr int  get_g() const noexcept{return (code>>3)&07;}
  constexpr int  get_b() const noexcept{return (code   )&07;}

  constexpr int  get_r255() const noexcept{return (((code>>6)&07)<<5)|0b11111;}
  constexpr int  get_g255() const noexcept{return (((code>>3)&07)<<5)|0b11111;}
  constexpr int  get_b255() const noexcept{return (((code   )&07)<<5)|0b11111;}

};


struct predefined{
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
  color_index  index=0;

  int16_t  z=0;

  constexpr pixel(color_index  i=color_index(), int16_t  z_=0) noexcept:
  index(i), z(z_){}

};


constexpr color_index
default_coloring[] = {0,predefined::white,0,0};


class
image
{
  int  m_width;
  int  m_height;

  std::vector<pixel>  m_pixels;

public:
  image() noexcept{}
  image(int  w, int  h) noexcept;


  void  resize(int  w, int  h) noexcept;

  void  load_png(binary_stream_reader&  r) noexcept;

  int   get_width() const noexcept{return m_width ;}
  int  get_height() const noexcept{return m_height;}

        pixel&        get_pixel(int  x, int  y)       noexcept{return m_pixels[m_width*y+x];}
  const pixel&  get_const_pixel(int  x, int  y) const noexcept{return m_pixels[m_width*y+x];}

  uint32_t  get_pixel_color(int  x, int  y, palette const&  pal) const noexcept;

  void  fill(pixel  pix=pixel()) noexcept;

  void  add(int  r, int  g, int  b) noexcept;
  void  reverse_color() noexcept;


  void  draw_dot(       point  pt, color_index  i) noexcept;
  void  draw_dot_safely(point  pt, color_index  i) noexcept;

  void  draw_dot(       point  pt, pixel  pix) noexcept;
  void  draw_dot_safely(point  pt, pixel  pix) noexcept;

  void  draw_vline(       point  pt, int  l, pixel  pix) noexcept;
  void  draw_vline_safely(point  pt, int  l, pixel  pix) noexcept;
  void  draw_hline(       point  pt, int  l, pixel  pix, int  interval=0) noexcept;
  void  draw_hline_safely(point  pt, int  l, pixel  pix) noexcept;

  void  draw_line(line  ln, pixel  pix) noexcept;

  void  draw_rectangle(       rectangle const&  rect, pixel  pix) noexcept;
  void  draw_rectangle_safely(rectangle const&  rect, pixel  pix) noexcept;
  void  fill_rectangle(       rectangle         rect, pixel  pix) noexcept;

  void  print(char16_t   c, point  pt, const font&  fnt, ro_ptr<color_index>  coloring=default_coloring) noexcept;
  void  print(gbstd::string_view     sv, point  pt, const font&  fnt, ro_ptr<color_index>  coloring=default_coloring) noexcept;
  void  print(gbstd::u16string_view  sv, point  pt, const font&  fnt, ro_ptr<color_index>  coloring=default_coloring) noexcept;

  void  transfer(image const&  src, rectangle  src_rect, point  dst_pt, int  z=0) noexcept;

};


}


using images::color_index;
using images::palette;
using images::predefined;

using images::image;
using images::pixel;


}




#endif




