#ifndef LIBGBSND_token_HPP
#define LIBGBSND_token_HPP


#include"libgbstd/string.hpp"
#include<vector>
#include<new>


namespace  gbscr{


class stream;


namespace tokens{


class block;


enum class
token_kind
{
  null,

  identifier,

  binary_integer,
  octal_integer,
  decimal_integer,
  hexadecimal_integer,

  string,

  punctuations,

  block,

};


class
token
{
  token_kind  m_kind=token_kind::null;

  union data{
    gbstd::string  str;
    block*         blk;

    data(){}
   ~data(){}

  } m_data;

  const char*  m_pointer=nullptr;

public:
  token() noexcept{}
  token(const token&   rhs) noexcept=delete;
  token(      token&&  rhs) noexcept{*this = std::move(rhs);}
 ~token(){unset_data();}

  token&  operator=(const token&   rhs) noexcept=delete;
  token&  operator=(      token&&  rhs) noexcept;

  operator bool() const noexcept{return m_kind != token_kind::null;}

  void  unset_data() noexcept;

  void         set_pointer(const char*  p)       noexcept{       m_pointer = p;}
  const char*  get_pointer(              ) const noexcept{return m_pointer    ;}

  bool  is_binary_integer()      const noexcept{return m_kind == token_kind::binary_integer;}
  bool  is_octal_integer()       const noexcept{return m_kind == token_kind::octal_integer;}
  bool  is_decimal_integer()     const noexcept{return m_kind == token_kind::decimal_integer;}
  bool  is_hexadecimal_integer() const noexcept{return m_kind == token_kind::hexadecimal_integer;}
  bool  is_identifier()          const noexcept{return m_kind == token_kind::identifier;}
  bool  is_string()              const noexcept{return m_kind == token_kind::string;}
  bool  is_block()               const noexcept{return m_kind == token_kind::block;}
  bool  is_punctuations()        const noexcept{return m_kind == token_kind::punctuations;}

  bool  is_identifier(gbstd::string_view  sv) const noexcept{return is_identifier() && (m_data.str == sv);}
  bool  is_punctuations(gbstd::string_view  sv) const noexcept{return is_punctuations() && (m_data.str == sv);}
  bool  is_block(char  open, char  close) const noexcept;

  void  set_kind(token_kind  k) noexcept{m_kind = k;}

  void  set_data(token_kind  k, gbstd::string&&  s) noexcept;
  void  set_data(block&  blk) noexcept;

  const gbstd::string&  get_string() const noexcept{return m_data.str;}
  const block&           get_block() const noexcept{return *m_data.blk;}

  void  print(int  indent=0) const noexcept;

};


class
block
{
  std::vector<token>  m_tokens;

  char  m_open=0;
  char  m_close=0;

public:
  block() noexcept{}
  block(stream&  s, char  open=0, char  close=0);

  bool  test(char  open, char  close) const noexcept{return((m_open == open) && (m_close == close));}

  const token*  begin() const noexcept{return m_tokens.data();}
  const token*    end() const noexcept{return m_tokens.data()+m_tokens.size();}

  void  print(int  indent=0) const noexcept;

};


class
cursor
{
  static const token  null;

  const token*  m_current=nullptr;
  const token*      m_end=nullptr;

public:
  cursor(const token*  begin, const token*  end) noexcept:
  m_current(begin),
  m_end(end){}

  cursor(const block&  blk) noexcept:
  m_current(blk.begin()),
  m_end(blk.end()){}

  operator bool() const noexcept{return m_current < m_end;}

  const token&  operator*() const noexcept;
  const token*  operator->() const noexcept;
  const token&  operator[](int  i) const noexcept;

  cursor&  operator+=(int  n) noexcept;
  cursor&  operator++(      ) noexcept;
  cursor   operator++(int  n) noexcept;

};


}


using tokens::token;
using tokens::token_kind;
using tokens::block;
using tokens::cursor;


}




#endif




