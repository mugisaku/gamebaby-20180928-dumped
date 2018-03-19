#ifndef stream_HPP
#define stream_HPP


#include<cstddef>
#include<cstdint>
#include<string>
#include"libgbscr/token.hpp"


namespace gbscr{


class
stream
{
  const char*  m_pointer=nullptr;

  token  m_token;

  void       read_binary_number() noexcept;
  void        read_octal_number() noexcept;
  void      read_decimal_number() noexcept;
  void  read_hexadecimal_number() noexcept;
  void  read_number_that_begins_by_zero() noexcept;

  void  read_number() noexcept;

  void  skip_linestyle_comment() noexcept;
  void  skip_blockstyle_comment();

  void  read_quoted_string(char  close_char) noexcept;

  bool  read_punct(gbstd::string_view  sv) noexcept;

public:
  stream() noexcept{}
  stream(const char*  s) noexcept: m_pointer(s){}

  const char*  get_pointer() const noexcept{return m_pointer;}

  const char&  operator*() const noexcept{return *m_pointer;}

  char  get_char() const noexcept{return *m_pointer;}

  token  read_token();

  operator bool() const noexcept{return *m_pointer;}

  void  skip_spaces();

  void  advance() noexcept{++m_pointer;}

};


void  print(const char*  start, const char*  cursor) noexcept;


}




#endif




