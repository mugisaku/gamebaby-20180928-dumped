#ifndef GMBB_text_HPP
#define GMBB_text_HPP


#include<string>
#include<cstdarg>
#include"libgbstd/unicode.hpp"
#include"libgbstd/string.hpp"
#include<initializer_list>


namespace gbstd{
namespace texts{


gbstd::string_view  make_text(const char*  fmt, ...) noexcept;
gbstd::string_view  make_text_with_va_list(const char*  fmt, va_list  ap) noexcept;


class
text_buffer
{
  char*  m_data_source=nullptr;

  size_t  m_data_length=0;

  char*   m_input_pointer=nullptr;

  utf8_decoder  m_decoder;

public:
  text_buffer() noexcept{}
  text_buffer(size_t  length) noexcept{resize(length);}
  text_buffer(const text_buffer&   rhs) noexcept=delete;
  text_buffer(      text_buffer&&  rhs) noexcept=delete;
 ~text_buffer(){clear();}

  text_buffer&  operator=(const text_buffer&   rhs) noexcept=delete;
  text_buffer&  operator=(      text_buffer&&  rhs) noexcept=delete;

  void  clear() noexcept;
  void  reset() noexcept;

  void  resize(size_t  length) noexcept;

  bool  is_remaining() const noexcept{return(m_decoder.get_pointer() < m_input_pointer);}

  void  push(gbstd::string_view  sv, bool  with_newline=true);

  char16_t  pop() noexcept;

  const char*  begin() const noexcept{return m_decoder.get_pointer();}
  const char*    end() const noexcept{return m_decoder.get_end();}

};


class
text_roll
{
public:
  struct line{
    char16_t*    begin;
    char16_t*  current;
    char16_t*      end;

    line*  next;
  };


  class iterator{
    const line*  m_line;

  public:
    iterator(const line*  line=nullptr) noexcept: m_line(line){}

    gbstd::u16string_view  operator*() const noexcept;

    bool  operator!=(const iterator&  rhs) const noexcept;

    iterator&  operator++() noexcept;

  };

private:
  int  m_number_of_columns=0;
  int  m_number_of_rows=0;

  char16_t  m_last_char=0;

  char16_t*  m_data_source=nullptr;
  char16_t*  m_data_source_end;
  line*      m_line_source=nullptr;

  line*    m_first;
  line*  m_current;
  line*     m_last;

public:
  text_roll(                      ) noexcept{}
  text_roll(int  col_n, int  row_n) noexcept{resize(col_n,row_n);}
  text_roll(const text_roll&   rhs) noexcept=delete;
  text_roll(      text_roll&&  rhs) noexcept=delete;
 ~text_roll(){clear();}

  text_roll&  operator=(const text_roll&   rhs) noexcept=delete;
  text_roll&  operator=(      text_roll&&  rhs) noexcept=delete;

  int  get_number_of_columns() const noexcept{return m_number_of_columns;}
  int  get_number_of_rows() const noexcept{return m_number_of_rows;}

  void  clear() noexcept;

  void  reset() noexcept;

  void  resize(int  col_n, int  row_n) noexcept;

  void  rotate() noexcept;

  void  push(char16_t  c) noexcept;

  bool  is_full() const noexcept;

  iterator  begin() const noexcept{return iterator(m_first);}
  iterator    end() const noexcept{return iterator(       );}

};


}


using texts::make_text;
using texts::make_text_with_va_list;
using texts::text_buffer;
using texts::text_roll;


}




#endif




