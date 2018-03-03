#ifndef LIBGBSND_token_HPP
#define LIBGBSND_token_HPP


#include<cstdio>
#include<vector>
#include<list>
#include<memory>
#include<initializer_list>
#include"libgbstd/string.hpp"
#include"libgbscr/shared_string.hpp"
#include"libgbscr/short_string.hpp"


namespace  gbscr{


class stream;


namespace tokens{


class  token;


struct semicolon{};


class
token_string
{
  token*  m_data=nullptr;

  size_t  m_length=0;

  char   m_open=0;
  char  m_close=0;

public:
  token_string() noexcept{}
  token_string(stream&  s, char  open, char  close);
  token_string(const token_string&   rhs) noexcept{*this = rhs;}
  token_string(      token_string&&  rhs) noexcept{*this = std::move(rhs);}
 ~token_string(){clear();}

  token_string&  operator=(const token_string&   rhs) noexcept;
  token_string&  operator=(      token_string&&  rhs) noexcept;

  void  clear() noexcept;

  size_t  size() const noexcept{return m_length;}

  const token*  data() const noexcept{return m_data;}

  char  get_open()  const noexcept{return m_open;}
  char  get_close() const noexcept{return m_close;}

  const token*  begin() const noexcept;
  const token*    end() const noexcept;

  void  print(FILE*  f, int  indent) const noexcept;

};


class
identifier
{
  shared_string  m_string;

public:
  identifier(const shared_string&  s) noexcept: m_string(s){}

  const shared_string&  operator*() const noexcept{return m_string;}

};


class
token
{
  enum class kind{
    null,
    identifier,
    quoted_string,
    integer_literal,
    operator_word,
    token_string,
    semicolon,

  } m_kind=kind::null;

  union data{
    uint64_t  i;

    shared_string  s;

    operator_word  opw;

    token_string  toks;

    data(){}
   ~data(){}
  } m_data;


  const char*  m_pointer=nullptr;

public:
  token() noexcept{}
  token(const token&   rhs) noexcept{*this = rhs;}
  token(      token&&  rhs) noexcept{*this = std::move(rhs);}
 ~token(){clear();}

  token&  operator=(const token&   rhs) noexcept;
  token&  operator=(      token&&  rhs) noexcept;

  operator bool() const noexcept{return m_kind != kind::null;}

  void  unset_data() noexcept;

  void  set_data(uint64_t  i) noexcept;
  void  set_data(const shared_string&  s) noexcept;
  void  set_data(const identifier&  id) noexcept;
  void  set_data(operator_word  opw) noexcept;
  void  set_data(token_string&&  toks) noexcept;
  void  set_data(semicolon  semcol) noexcept;

  void  clear() noexcept;

  void         set_pointer(const char*  p)       noexcept{       m_pointer = p;}
  const char*  get_pointer(              ) const noexcept{return m_pointer    ;}

  bool  is_integer_literal()  const noexcept{return m_kind == kind::integer_literal;}
  bool  is_identifier()       const noexcept{return m_kind == kind::identifier;}
  bool  is_operator_word()    const noexcept{return m_kind == kind::operator_word;}
  bool  is_quoted_string()    const noexcept{return m_kind == kind::quoted_string;}
  bool  is_token_string()     const noexcept{return m_kind == kind::token_string;}
  bool  is_semicolon()        const noexcept{return m_kind == kind::semicolon;}

  using string_view_list = std::initializer_list<gbstd::string_view>;

  bool  is_identifier(string_view_list  ls)    const noexcept;
  bool  is_operator_word(string_view_list  ls) const noexcept;
  bool  is_token_string(char  open, char  close)  const noexcept;

  uint64_t              get_integer()       const noexcept{return m_data.i;}
  const shared_string&  get_string()        const noexcept{return m_data.s;}
  operator_word         get_operator_word() const noexcept{return m_data.opw;}
  const token_string&   get_token_string()  const noexcept{return m_data.toks;}

  void  print(FILE*  f, int  indent) const noexcept;

};


class
token_cursor
{
  static const token  null;

  const token*  m_current=nullptr;
  const token*      m_end=nullptr;

public:
  token_cursor(const token_string&  toks) noexcept:
  m_current(toks.begin()),
  m_end(toks.end()){}

  token_cursor(const token*  current, const token*  end) noexcept:
  m_current(current),
  m_end(end){}

  operator bool() const noexcept{return m_current < m_end;}

  const token&  operator*() const noexcept;
  const token*  operator->() const noexcept;
  const token&  operator[](int  i) const noexcept;

  token_cursor&  operator+=(int  n) noexcept;
  token_cursor&  operator++(      ) noexcept;
  token_cursor   operator++(int  n) noexcept;

};


}


using tokens::semicolon;
using tokens::identifier;
using tokens::token;
using tokens::token_string;
using tokens::token_cursor;


}




#endif




