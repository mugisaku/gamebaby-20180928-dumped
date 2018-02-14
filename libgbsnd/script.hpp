#ifndef LIBGBSND_SCRIPT_HPP
#define LIBGBSND_SCRIPT_HPP


#include"libgbsnd/shared_string.hpp"
#include"libgbsnd/short_string.hpp"
#include"libgbsnd/device.hpp"
#include"libgbsnd/stmt.hpp"
#include"libtok/stream_reader.hpp"
#include<cstdio>
#include<vector>
#include<list>
#include<memory>
#include<initializer_list>


namespace gbsnd{
namespace scripts{


class script_token;


struct semicolon{};


class
script_token_string
{
  script_token*  m_data=nullptr;

  size_t  m_length=0;

  char  m_open=0;
  char  m_close=0;

public:
  script_token_string() noexcept{}
  script_token_string(tok::stream_reader&  r, char  open, char  close);
  script_token_string(const script_token_string&   rhs) noexcept{*this = rhs;}
  script_token_string(      script_token_string&&  rhs) noexcept{*this = std::move(rhs);}
 ~script_token_string(){clear();}

  script_token_string&  operator=(const script_token_string&   rhs) noexcept;
  script_token_string&  operator=(      script_token_string&&  rhs) noexcept;

  void  clear() noexcept;

  size_t  size() const noexcept{return m_length;}

  const script_token*  data() const noexcept{return m_data;}

  char  get_open()  const noexcept{return m_open;}
  char  get_close() const noexcept{return m_close;}

  const script_token*  begin() const noexcept;
  const script_token*    end() const noexcept;

  void  print(FILE*  f, int  indent) const noexcept;

};


class
script_token
{
  enum class kind{
    null,
    identifier,
    integer_literal,
    operator_word,
    token_string,
    semicolon,

  } m_kind=kind::null;

  union data{
    uint64_t  i;

    identifier  id;

    operator_word  opw;

    script_token_string  toks;

    data(){}
   ~data(){}
  } m_data;


  tok::stream_context  m_stream_context;

public:
  script_token() noexcept{}
  script_token(uint64_t  i) noexcept{*this = i;}
  script_token(const identifier&  id) noexcept{*this = id;}
  script_token(operator_word  opw) noexcept{*this = opw;}
  script_token(script_token_string&&  toks) noexcept{*this = std::move(toks);}
  script_token(semicolon  semcol) noexcept{*this = semcol;}
  script_token(const script_token&   rhs) noexcept{*this = rhs;}
  script_token(      script_token&&  rhs) noexcept{*this = std::move(rhs);}
 ~script_token(){clear();}

  script_token&  operator=(uint64_t  i) noexcept;
  script_token&  operator=(const identifier&  id) noexcept;
  script_token&  operator=(operator_word  opw) noexcept;
  script_token&  operator=(script_token_string&&  toks) noexcept;
  script_token&  operator=(semicolon  semcol) noexcept;
  script_token&  operator=(const script_token&   rhs) noexcept;
  script_token&  operator=(      script_token&&  rhs) noexcept;

  void  clear() noexcept;

  void  set_stream_context(const tok::stream_context&  sctx) noexcept{m_stream_context = sctx;}
  const tok::stream_context&  get_stream_context() const noexcept{return m_stream_context;}

  bool  is_integer_literal()  const noexcept{return m_kind == kind::integer_literal;}
  bool  is_identifier()       const noexcept{return m_kind == kind::identifier;}
  bool  is_operator_word()    const noexcept{return m_kind == kind::operator_word;}
  bool  is_token_string()     const noexcept{return m_kind == kind::token_string;}
  bool  is_semicolon()        const noexcept{return m_kind == kind::semicolon;}

  using string_view_list = std::initializer_list<gbstd::string_view>;

  bool  is_identifier(string_view_list  ls)    const noexcept;
  bool  is_operator_word(string_view_list  ls) const noexcept;
  bool  is_token_string(char  open, char  close)  const noexcept;

  uint64_t                    get_integer()       const noexcept{return m_data.i;}
  const identifier&           get_identifier()    const noexcept{return m_data.id;}
  operator_word               get_operator_word() const noexcept{return m_data.opw;}
  const script_token_string&  get_token_string()  const noexcept{return m_data.toks;}

  void  print(FILE*  f, int  indent) const noexcept;

};


class
script_token_cursor
{
  static const script_token  null;

  const script_token*  m_current=nullptr;
  const script_token*      m_end=nullptr;

public:
  script_token_cursor(const script_token_string&  toks) noexcept:
  m_current(toks.begin()),
  m_end(toks.end()){}

  script_token_cursor(const script_token*  current, const script_token*  end) noexcept:
  m_current(current),
  m_end(end){}

  operator bool() const noexcept{return m_current < m_end;}

  const script_token&  operator*() const noexcept;
  const script_token*  operator->() const noexcept;
  const script_token&  operator[](int  i) const noexcept;

  script_token_cursor&  operator+=(int  n) noexcept;
  script_token_cursor&  operator++(      ) noexcept;
  script_token_cursor   operator++(int  n) noexcept;

};


class
script
{
  struct data;

  data*  m_data=nullptr;

  script(const script_token_string&  toks, gbstd::string_view  source_path) noexcept;

  void  unrefer() noexcept;

public:
  script() noexcept{}
  script(const script&   rhs) noexcept{*this = rhs;}
  script(      script&&  rhs) noexcept{*this = std::move(rhs);}
 ~script(){unrefer();}

  script&  operator=(const script&   rhs) noexcept;
  script&  operator=(      script&&  rhs) noexcept;

  const gbstd::string&  get_source_path() const noexcept;

  std::list<object>&                                     get_object_list() const noexcept;
  const std::vector<std::unique_ptr<routine>>&          get_routine_list() const noexcept;
  const std::vector<std::unique_ptr<square_wave>>&  get_square_wave_list() const noexcept;
  const std::vector<std::unique_ptr<noise>>&              get_noise_list() const noexcept;

  const routine*  find_routine(gbstd::string_view  name) const noexcept;

  void  print() const noexcept;

  static script  build_from_string(gbstd::string_view  sv, gbstd::string_view  source_path) noexcept;
  static script  build_from_file(gbstd::string_view  path) noexcept;
  static script  build_from_file(FILE*  f, gbstd::string_view  source_path) noexcept;

};


}


using scripts::script;
using scripts::script_token;
using scripts::script_token_string;
using scripts::script_token_cursor;


}




#endif




