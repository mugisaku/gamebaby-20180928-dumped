#ifndef stream_HPP
#define stream_HPP


#include<cstddef>
#include<cstdint>
#include<string>
#include"libgbstd/string.hpp"
#include"libgbscr/token.hpp"
#include"libgbscr/shared_string.hpp"


namespace gbscr{


class
stream
{
  const char*  m_pointer=nullptr;

  std::string  m_string_buffer;

  token  m_token;

  uint64_t       read_binary_number() noexcept;
  uint64_t        read_octal_number() noexcept;
  uint64_t      read_decimal_number() noexcept;
  uint64_t  read_hexadecimal_number() noexcept;
  uint64_t  read_number_that_begins_by_zero() noexcept;

  void  skip_linestyle_comment() noexcept;
  void  skip_blockstyle_comment();

  identifier     read_identifier() noexcept;
  shared_string  read_quoted_string(char  close_char) noexcept;

  bool  read_operator(gbstd::string_view  sv) noexcept;

  uint64_t  read_number() noexcept;

public:
  stream() noexcept{}
  stream(const char*  s) noexcept: m_pointer(s){}

  const char*  get_pointer() const noexcept{return m_pointer;}

  const char&  operator*() const noexcept{return *m_pointer;}

  char  get_char() const noexcept{return *m_pointer;}

  const token&  get_token() const noexcept{return m_token;}

  const token&  read_token();

  operator bool() const noexcept{return *m_pointer;}

  void  skip_spaces();

  void  advance() noexcept{++m_pointer;}

};


void  print(const char*  start, const char*  cursor) noexcept;


}




#endif




