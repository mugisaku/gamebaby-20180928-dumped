#ifndef stream_HPP
#define stream_HPP


#include<cstddef>
#include<cstdint>
#include<string>
#include"token.hpp"


namespace gbscr{


class
stream
{
  const char*  m_pointer=nullptr;
  const char*      m_end=nullptr;

  std::string  m_string_buffer;

  uint64_t       read_binary_number() noexcept;
  uint64_t        read_octal_number() noexcept;
  uint64_t      read_decimal_number() noexcept;
  uint64_t  read_hexadecimal_number() noexcept;
  uint64_t  read_number_that_begins_by_zero() noexcept;

  void  skip_linestyle_comment() noexcept;
  void  skip_blockstyle_comment();

  token  read_identifier() noexcept;
  token  read_quoted_string(char  close_char) noexcept;

  token  read_number() noexcept;

  void  newline() noexcept;

public:
  stream() noexcept{}
  stream(const char*  s, size_t  l) noexcept: m_pointer(s), m_end(s+l){}

  const char*  get_pointer() const noexcept{return m_pointer;}

  char  get_char() const noexcept{return is_reached_end()? 0:*m_pointer;}

  void  advance() noexcept{++m_pointer;}

  bool  is_pointing_identifier() const noexcept;
  bool  is_pointing_number() const noexcept;

  bool  is_reached_end() const noexcept{return !*m_pointer || (m_pointer >= m_end);}

  void  skip_spaces();

};


void  print(const char*  start, const char*  cursor) noexcept;


}




#endif




