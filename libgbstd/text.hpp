#ifndef GMBB_text_HPP
#define GMBB_text_HPP


#include<string>
#include<vector>
#include<cstdarg>
#include"libgbstd/string.hpp"
#include"libgbstd/figures.hpp"
#include<initializer_list>


namespace gbstd{
namespace texts{


class
utf8_decoder
{
  const char*  m_pointer=nullptr;
  const char*  m_end=nullptr;

public:
  utf8_decoder(                      ) noexcept{}
  utf8_decoder(gbstd::string_view  sv) noexcept{*this = sv;}

  utf8_decoder&  operator=(gbstd::string_view  sv) noexcept
  {
    m_pointer = sv.data();
    m_end     = sv.data()+sv.size();

    return *this;
  }

  operator bool() const noexcept{return m_pointer < m_end;}

  char32_t  operator()() noexcept;

  const char*  get_pointer() const noexcept{return m_pointer;}
  const char*  get_end() const noexcept{return m_end;}

};


size_t  u8slen(gbstd::string_view  sv) noexcept;


struct
utf8_encoder
{
  char  codes[8];

  utf8_encoder(char32_t  c=0) noexcept{(*this)(c);}

  utf8_encoder&  operator()(char32_t  c) noexcept;

};


class
character_queue
{
  static constexpr size_t  buffer_size = 1024;

  char  m_data[buffer_size];

  char*  m_input_pointer=nullptr;

  utf8_decoder  m_decoder;

public:
  character_queue() noexcept{clear();}
  character_queue(const character_queue&   rhs) noexcept=delete;
  character_queue(      character_queue&&  rhs) noexcept=delete;

  character_queue&  operator=(const character_queue&   rhs) noexcept=delete;
  character_queue&  operator=(      character_queue&&  rhs) noexcept=delete;

  operator bool() const noexcept{return(m_decoder.get_pointer() < m_input_pointer);}

  void  clear() noexcept;

  void  push(gbstd::string_view  sv, bool  with_newline=true);

  char16_t  pop() noexcept;

  const char*  begin() const noexcept{return m_decoder.get_pointer();}
  const char*    end() const noexcept{return m_decoder.get_end();}

};


class
text
{
  int  m_width =0;
  int  m_height=0;

  std::vector<char16_t>  m_chars;

public:
  text(                      ) noexcept{}
  text(int  w, int  h) noexcept{resize(w,h);}

  int  get_width()  const noexcept{return m_width;}
  int  get_height() const noexcept{return m_height;}

        char16_t&  get_char(int  x, int  y)       noexcept{return m_chars[m_width*y+x];}
  const char16_t&  get_char(int  x, int  y) const noexcept{return m_chars[m_width*y+x];}

  void  resize(int  w, int  h) noexcept;

  void  fill(char16_t  c) noexcept{std::fill(m_chars.begin(),m_chars.end(),c);}

};




class
string_form
{
  char  buf[256];

public:
  string_form() noexcept: buf{0}{}
  string_form(const char*  fmt, ...) noexcept;

  const char*  operator*() const noexcept{return buf;}

  const char*  operator()(const char*  fmt, ...) noexcept;

};


}


using texts::utf8_decoder;
using texts::utf8_encoder;
using texts::u8slen;
using texts::text;
using texts::character_queue;
using texts::string_form;


}




#endif




