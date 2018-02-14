#ifndef GMBB_font_HPP_INCLUDED
#define GMBB_font_HPP_INCLUDED


#include<cstdint>
#include<cstdio>
#include<cstddef>
#include"libgbstd/binary_stream.hpp"


namespace gbstd{
namespace fonts{


class
font
{
  uint32_t*  m_link_table[0x10000-256]={0};

  int   m_width=0;
  int  m_height=0;

  int  m_bits_per_pixel=0;

public:
  font() noexcept{}
  font(const font&   rhs) noexcept=delete;
  font(      font&&  rhs) noexcept=delete;
 ~font(){clear();}


  font&  operator=(const font&   rhs) noexcept=delete;
  font&  operator=(      font&&  rhs) noexcept=delete;

  void  clear() noexcept;

  int   get_width() const noexcept{return m_width ;}
  int  get_height() const noexcept{return m_height;}
  int  get_bits_per_pixel() const noexcept{return m_bits_per_pixel;}

  const uint32_t*  get_glyph_data(char16_t  c) const noexcept;

  void  load_from_file(gbstd::string_view  path)       noexcept;
  void    save_to_file(gbstd::string_view  path) const noexcept;

  void  load_from_stream(binary_stream_reader&  r)       noexcept;
  void    save_to_stream(binary_stream_writer&  w) const noexcept;

};


}


using fonts::font;


}


#endif




